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

int hmm::next_observation() {
  int state = next_state();
  int observation = mc_random_.choose_dirichlet(emission_p_.col(state));
  return observation;
}

double hmm::likelihood(const std::vector<int> &observation,
                       const std::vector<int> &hidden_trace) {
  return 0;
}

// Parameter estimation: baum-welch
Eigen::MatrixXd hmm::forward_procedure(const std::vector<int> &observation) {
  auto alpha = Eigen::MatrixXd();
  for (int i = 0; i < )

  return alpha;
}

Eigen::MatrixXd hmm::backward_procedure(const std::vector<int> &observation) {
  auto beta = Eigen::MatrixXd();


  return beta;
}

double hmm::step(const int &i, const int &j, const int &t) {
  return 0.0;
}

void hmm::fit(const std::vector<int> &observation, const int max_iters) {
  for (int i = 0; i < max_iters; i++) {
    auto alpha = forward_procedure(observation);
    // TODO: reestimate initial state distribution
    // TODO: reestimate transition matrix
    // TODO: reestimate emission matrix
  }
}

// Observation explanation: viterbi
std::vector<int> explain(const std::vector<int> &observation) {}
