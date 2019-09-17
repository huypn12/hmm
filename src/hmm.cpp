#include "hmm.hpp"
#include "mc_random.hpp"

using namespace org::mcss;

hmm::hmm(const int &states_size, const int &alphabet_count)
    : dtmc(states_size),
      alphabet_count_(alphabet_count),
      emission_p_{} {
  emission_p_ = Eigen::MatrixXd::Zero(states_size, alphabet_count);
}

hmm::hmm(const int &state_count, const int &alphabet_count,
         const Eigen::VectorXd &initial_p,
         const Eigen::MatrixXd &transition_p,
         const Eigen::MatrixXd &emission_p)
  : dtmc(state_count, initial_p, transition_p),
    alphabet_count_(alphabet_count),
    emission_p_{} {
  emission_p_ = emission_p;
}

void hmm::init_random() {
  initial_p_ = mc_random_.random_vector(state_count_);
  transition_p_ = mc_random_.random_matrix(state_count_, state_count_);
  emission_p_ = mc_random_.random_matrix(state_count_, alphabet_count_);
}

std::string hmm::model_info() {
  std::stringstream ss;
  ss << "* Model parameters: " << std::endl;
  ss << "Hidden states cardinality: " << state_count_ << std::endl;
  ss << "Observation cardinality: " << alphabet_count_ << std::endl;

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
  auto alpha = Eigen::MatrixXd(state_count_, T);
  // basis step
  alpha.col(0) = initial_p_.cwiseProduct(emission_p_.col(observation[0]));
  alpha.col(0) = alpha.col(0) / alpha.col(0).sum();
  // inductive step
  for (int t = 1; t < T; t++) {
    alpha.col(t) = alpha.col(t - 1).transpose() * transition_p_;
    alpha.col(t) = alpha.col(t).cwiseProduct(emission_p_.col(observation[t]));
    alpha.col(t) = alpha.col(t) / alpha.col(t).sum();
  }
  return alpha;
}

Eigen::MatrixXd hmm::backward(const std::vector<int> &observation) {
  auto T = observation.size();
  auto beta = Eigen::MatrixXd(state_count_, T);
  // basis step
  beta.col(T - 1) =
      Eigen::VectorXd(state_count_)
          .setConstant(1 / emission_p_.col(observation[T - 1]).sum());
  // inductive step
  for (int t = T - 2; t >= 0; t--) {
    beta.col(t) = beta.col(t + 1).cwiseProduct(emission_p_.col(observation[t + 1]));
    beta.col(t) = (transition_p_ * beta.col(t)).transpose();
    auto sum = beta.col(t).cwiseProduct(emission_p_.col(observation[t])).sum();
    beta.col(t) = beta.col(t) / sum;
  }
  return beta;
}

Eigen::MatrixXd hmm::posterior(const std::vector<int> &observation) {
  auto alpha = forward(observation);
  auto beta = backward(observation);
  return posterior(observation, alpha, beta);
}

Eigen::MatrixXd hmm::posterior(const std::vector<int> &observation,
                               const Eigen::MatrixXd &alpha,
                               const Eigen::MatrixXd &beta) {
  auto T = observation.size();
  auto gamma = Eigen::MatrixXd(state_count_, T);
  for (int t = 0; t < T; t++) {
    gamma.col(t) = alpha.col(t).cwiseProduct(beta.col(t));
    auto sum = gamma.col(t).sum();
    gamma.col(t) = gamma.col(t) / gamma.col(t).sum();
  }
  return gamma;
}

void hmm::expectation(const std::vector<int> &observation,
                      Eigen::MatrixXd &gamma, Eigen::MatrixXd &sigma_xi) {
  auto T = observation.size();
  auto alpha = forward(observation);
  auto beta = backward(observation);
  gamma = posterior(observation, alpha, beta);
  sigma_xi = Eigen::MatrixXd::Zero(state_count_, state_count_);
  for (int t = 0; t < T - 1; t++) {
    auto xi = Eigen::MatrixXd(transition_p_);
    xi = xi.array().colwise() * alpha.col(t).array();
    xi = xi.array().colwise() * emission_p_.col(observation[t + 1]).array();
    xi = xi.array().rowwise() * beta.col(t + 1).transpose().array();
    xi = xi / xi.sum();
    sigma_xi += xi;
  }
  //TODO: asssert log likelihood is the same among all columns
  log_likelihood_ = log(alpha.col(T - 2).cwiseProduct(beta.col(T - 2)).sum());
  auto param_count = state_count_ * state_count_ +
                     state_count_ * alphabet_count_ + state_count_;
  aic_ = -2 * log_likelihood_ + 2 * param_count;
}

double hmm::maximization(const std::vector<int> &observation,
                         const Eigen::MatrixXd &gamma,
                         const Eigen::MatrixXd &sigma_xi) {
  auto T = observation.size();

  Eigen::VectorXd new_initial = gamma.rowwise().sum() / observation.size();
  Eigen::MatrixXd new_transition =
      sigma_xi.array().colwise() / gamma.rowwise().sum().array();
  Eigen::MatrixXd new_emission =
      Eigen::MatrixXd::Zero(state_count_, alphabet_count_);
  for (int t = 0; t < T; t++) {
    auto o = observation[t];
    new_emission.col(o) += gamma.col(t);
  }
  new_emission = new_emission.array().colwise() / gamma.rowwise().sum().array();

  auto norm_diff = 0.0;
  norm_diff += (new_initial - initial_p_).norm();
  norm_diff += (new_transition - transition_p_).norm();
  norm_diff += (new_emission - emission_p_).norm();


  initial_p_ = new_initial;
  transition_p_ = new_transition;
  emission_p_ = new_emission;

  return norm_diff;
}

void hmm::fit(const std::vector<int> &observation, const int max_iters,
              const double eps) {
  auto T = observation.size();
  for (int i = 0; i < max_iters; i++) {
    Eigen::MatrixXd gamma(state_count_, T);
    Eigen::MatrixXd sigma_xi(state_count_, state_count_);
    expectation(observation, gamma, sigma_xi);
    auto norm_diff = maximization(observation, gamma, sigma_xi);
    //TODO: assert log likelihood increases
    if (norm_diff <= eps) {
      break;
    }
  }
}

// Observation explanation: viterbi
std::vector<int> decode(const std::vector<int> &observation) {
  // TODO: implement viterbi algorithm
  return observation;
}

