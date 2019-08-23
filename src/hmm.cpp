#include "hmm.hpp"
#include "mc_random.hpp"

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
  auto alpha = Eigen::MatrixXd(states_size_, T);
  // basis step
  auto o_0 = emission_p_.col(observation[0]).asDiagonal().toDenseMatrix();
  alpha.col(0) = initial_p_.transpose() * transition_p_ * o_0;
  // inductive step
  for (int t = 1; t < T; t++) {
    auto O_t = emission_p_.col(observation[t]).asDiagonal().toDenseMatrix();
    auto alpha_t = alpha.col(t - 1).transpose() * transition_p_ * O_t;
    auto c_t = alpha_t.sum();
    alpha.col(t) = (1 / c_t) * alpha_t;
  }
  return alpha;
}

Eigen::MatrixXd hmm::backward(const std::vector<int> &observation) {
  auto T = observation.size();
  auto beta = Eigen::MatrixXd(states_size_, T);
  // basis step
  beta.col(T - 1).setOnes();
  beta.col(T - 1) = beta.col(T - 1) / beta.col(T - 1).sum();
  // inductive step
  for (int t = T - 2; t >= 0; t--) {
    auto O_t = emission_p_.col(observation[t]).asDiagonal().toDenseMatrix();
    auto beta_t = transition_p_ * O_t * beta.col(t + 1);
    auto c_t = beta_t.sum();
    beta.col(t) = (1 / c_t) * beta_t;
  }
    return beta;
}

Eigen::MatrixXd hmm::posterior(const std::vector<int> &observation) {
  auto T = observation.size();
  auto gamma = Eigen::MatrixXd(states_size_, T);
  auto alpha = forward(observation);
  auto beta = backward(observation);
  gamma = alpha.cwiseProduct(beta);
  return gamma;
}

// Parameter estimation: baum-welch
void hmm::expectation(Eigen::MatrixXd &gamma, Eigen::MatrixXd &p) {
  
}

void hmm::maximization(const Eigen::MatrixXd &gamma, const Eigen::MatrixXd &p,
                  Eigen::VectorXd &new_initial, Eigen::MatrixXd &new_transition,
                  Eigen::MatrixXd &new_emission) {
}

void hmm::fit(const std::vector<int> &observation, const int max_iters) {
  auto T = observation.size();

  Eigen::VectorXd new_initial_p(states_size_);
  Eigen::MatrixXd new_transition_p(states_size_, states_size_);
  Eigen::MatrixXd new_emission_p(states_size_, alphabet_size_);
  for (int i = 0; i < max_iters; i++) {
    Eigen::MatrixXd gamma_sum(states_size_, T);
    Eigen::MatrixXd p_sum(states_size_, states_size_);
    expectation(gamma_sum, p_sum);
    maximization(gamma_sum, p_sum,
                 new_initial_p, new_transition_p, new_emission_p);
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
