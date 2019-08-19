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

// SIMULATE
int hmm::next_observation() {
  int state = next_state();
  int observation = mc_random_.choose_dirichlet(emission_p_.col(state));
  return observation;
}

// INFERENCE
// Question 1
Eigen::MatrixXd hmm::forward_procedure(const std::vector<int> &observation) {
  auto observation_len = observation.size();
  auto alpha = Eigen::MatrixXd(states_size_, observation_len);
  // anchor step
  alpha.col(0) = initial_p_ * emission_p_.col(observation[0]);
  // inductive step
  for (int t = 1; t < observation_len; t++) {
    alpha.col(t) = emission_p_.col(observation[t]);
  }

  return alpha;
}

Eigen::MatrixXd hmm::backward_procedure(const std::vector<int> &observation) {
  auto observation_len = observation.size();
  auto beta = Eigen::MatrixXd();
  for (int i = 0; i < observation_len; i++) {
  }
  return beta;
}

Eigen::MatrixXd hmm::smooth(const std::vector<int> &observation) {
  return Eigen::MatrixXd();
}

// Parameter estimation: baum-welch

double hmm::step(const int &t, const int &i, const int &j) { return 0.0; }

void hmm::expectation(Eigen::MatrixXd &gamma, Eigen::MatrixXd &p) {
  
}

void hmm::maximization(const Eigen::MatrixXd &gamma, const Eigen::MatrixXd &p,
                  Eigen::VectorXd &new_initial, Eigen::MatrixXd &new_transition,
                  Eigen::MatrixXd &new_emission) {
}

void hmm::fit(const std::vector<int> &observation, const int max_iters) {
  auto observation_len = observation.size();

  Eigen::VectorXd new_initial_p(states_size_);
  Eigen::MatrixXd new_transition_p(states_size_, states_size_);
  Eigen::MatrixXd new_emission_p(states_size_, alphabet_size_);
  for (int i = 0; i < max_iters; i++) {
    Eigen::MatrixXd gamma_sum(states_size_, observation_len);
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
std::vector<int> explain(const std::vector<int> &observation) {
  //TODO: implement viterbi algorithm
  return observation;
}
