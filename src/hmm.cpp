#include "hmm.hpp"
#include "mc_random.hpp"

#include <iostream>

using namespace org::mcss;

hmm::hmm(const int &states_size, const int &alphabet_size)
    : dtmc(states_size,
           Eigen::VectorXd(states_size),
           Eigen::MatrixXd(states_size, states_size)),
      alphabet_size_(alphabet_size),
      emission_p_(Eigen::MatrixXd(states_size, alphabet_size)) {}

hmm::hmm(const int &states_size, const int &alphabet_size,
         const Eigen::VectorXd &p0, const Eigen::MatrixXd &p,
         const Eigen::MatrixXd &b)
    : dtmc(states_size, p0, p), alphabet_size_(alphabet_size), emission_p_(b) {}

void hmm::init_random() {
  initial_p_ = mc_random_.random_vector(states_size_);
  transition_p_ = mc_random_.random_matrix(states_size_, states_size_);
  emission_p_ = mc_random_.random_matrix(states_size_, alphabet_size_);
}

std::string hmm::model_info() {
  std::stringstream ss;
  ss << "* Model parameters: " << std::endl;
  ss << "Hidden states cardinality: " << states_size_ << std::endl;
  ss << "Observation cardinality: " << alphabet_size_ << std::endl;

  ss << "Initial distribution: \n" << initial_p_ << std::endl;
  ss << "Transition probabilities: \n" << transition_p_ << std::endl;
  ss << "Emission probabilities: \n" << emission_p_ << std::endl;

  ss << "* Model inferences: " << std::endl;
  ss << "Log likelihood: " << log_likelihood_ << std::endl;
  ss << "AIC: " << aic_ << std::endl;

  return ss.str();
}

// SIMULATE
int hmm::next_observation() {
  int state = next_state();
  int observation = mc_random_.choose_dirichlet(emission_p_.row(state));
  return observation;
}

// INFERENCE
// Question 1
Eigen::MatrixXd hmm::forward(const std::vector<int> &observation) {
  auto T = observation.size();
  auto alpha = Eigen::MatrixXd(states_size_, T + 1);
  // basis step
  alpha.col(0) = initial_p_;
  // inductive step
  for (int t = 1; t <= T; t++) {
    auto O_t = emission_p_.col(observation[t - 1]).asDiagonal().toDenseMatrix();
    auto alpha_t = alpha.col(t - 1).transpose() * transition_p_ * O_t;
    alpha.col(t) = alpha_t / alpha_t.sum();
  }
  return alpha;
}

Eigen::MatrixXd hmm::backward(const std::vector<int> &observation) {
  auto T = observation.size();
  auto beta = Eigen::MatrixXd(states_size_, T + 1);
  // basis step
  beta.col(T) = Eigen::VectorXd(states_size_).setOnes() / states_size_ ;
  // inductive step
  for (int t = T - 1; t >= 0; t--) {
    auto O_t = emission_p_.col(observation[t]).asDiagonal().toDenseMatrix();
    auto beta_t = transition_p_ * O_t * beta.col(t + 1);
    beta.col(t) = beta_t / beta_t.sum();
  }
  return beta;
}

Eigen::MatrixXd hmm::posterior(const std::vector<int> &observation) {
  auto T = observation.size();
  auto gamma = Eigen::MatrixXd(states_size_, T + 1);
  auto alpha = forward(observation);
  auto beta = backward(observation);
  for (int t = 0; t <= T; t++) {
    gamma.col(t) = alpha.col(t).cwiseProduct(beta.col(t));
    gamma.col(t) = gamma.col(t) / gamma.col(t).sum();
  }
  return gamma;
}

Eigen::MatrixXd hmm::posterior(const std::vector<int> &observation,
                               const Eigen::MatrixXd &alpha,
                               const Eigen::MatrixXd &beta) {
  auto T = observation.size();
  auto gamma = Eigen::MatrixXd(states_size_, T + 1);
  for (int t = 0; t <= T; t++) {
    gamma.col(t) = alpha.col(t).cwiseProduct(beta.col(t));
    gamma.col(t) = gamma.col(t) / gamma.col(t).sum();
  }
  return gamma;
}

void hmm::expectation(const std::vector<int> &observation,
                      Eigen::MatrixXd &gamma,
                      Eigen::MatrixXd &sigma_xi) {
  auto T = observation.size();
  auto alpha = forward(observation);
  auto beta = backward(observation);
  gamma = posterior(observation, alpha, beta);
  sigma_xi = Eigen::MatrixXd(states_size_, states_size_);
  for (int t = 0; t < T; t++) {
    auto xi = Eigen::MatrixXd(states_size_, states_size_);
    auto transit_alpha = transition_p_.array().colwise() * alpha.col(t).array();
    auto emit_beta = emission_p_.col(observation[t+1]).cwiseProduct(beta.col(t + 1));
    xi = transit_alpha.array().rowwise() * emit_beta.transpose().array();
    xi = xi / xi.sum();
    sigma_xi += xi;
  }
}

double hmm::maximization(const std::vector<int> &observation,
                       const Eigen::MatrixXd &gamma,
                       const Eigen::MatrixXd &sigma_xi,
                       Eigen::VectorXd &initial,
                       Eigen::MatrixXd &transition,
                       Eigen::MatrixXd &emission) {
  Eigen::VectorXd new_initial = gamma.col(0);
  Eigen::MatrixXd new_transition =
      sigma_xi.array().colwise() *
      gamma.block(0, 0, gamma.rows(), gamma.cols() - 1).colwise().sum().transpose().array();
  Eigen::MatrixXd new_emission = Eigen::MatrixXd(states_size_, alphabet_size_);
  for (int t = 0; t < observation.size(); t++) {
    auto o = observation[t];
    new_emission.col(o) += gamma.col(t);
  }
  new_emission = new_emission.array().colwise() / gamma.rowwise().sum().array();
  // relative difference
  auto norm_diff = 0.0;
  norm_diff += (new_initial - initial).norm();
  norm_diff += (new_transition - transition).norm();
  norm_diff += (new_emission - emission).norm();

  return norm_diff;
}

void hmm::fit(const std::vector<int> &observation, const int max_iters, const double eps) {
  auto T = observation.size();

  Eigen::VectorXd new_initial_p(states_size_);
  Eigen::MatrixXd new_transition_p(states_size_, states_size_);
  Eigen::MatrixXd new_emission_p(states_size_, alphabet_size_);
  double new_log_likelihood;

  for (int i = 0; i < max_iters; i++) {
    Eigen::MatrixXd gamma_sum(states_size_, T);
    Eigen::MatrixXd p_sum(states_size_, states_size_);
    expectation(observation, gamma_sum, p_sum);
    auto diff = maximization(observation, gamma_sum, p_sum,
                 new_initial_p, new_transition_p, new_emission_p);
    // stopping criteria
    if (diff <= eps) {
      break;
    }
  }


  initial_p_ = new_initial_p;
  transition_p_ = new_transition_p;
  emission_p_ = new_emission_p;
}

// Observation explanation: viterbi
std::vector<int> decode(const std::vector<int> &observation) {
  //TODO: implement viterbi algorithm
  return observation;
}
