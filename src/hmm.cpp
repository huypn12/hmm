#include "hmm.hpp"
#include "mc_random.hpp"

using namespace org::mcss;

hmm::hmm(const int &states_size, const int &alphabet_size)
    : dtmc(states_size, Eigen::VectorXd(states_size),
           Eigen::MatrixXd(states_size, states_size)),
      alphabet_size_(alphabet_size),
      emission_p_(Eigen::MatrixXd(states_size, alphabet_size)) {}

hmm::hmm(const int &states_size, const int &alphabet_size,
         const Eigen::VectorXd &p0, const Eigen::MatrixXd &p,
         const Eigen::MatrixXd &b)
    : dtmc(states_size, p0, p), alphabet_size_(alphabet_size), emission_p_(b) {}

Eigen::VectorXd hmm::random_vector(const int &dim) {
  auto vector = Eigen::VectorXd(dim);
  for (int i = 0; i < vector.size(); i++) {
    vector(i) = mc_random_.uniform_p();
  }
  return vector;
}

Eigen::MatrixXd hmm::random_matrix(const int &row, const int &col) {
  auto matrix = Eigen::MatrixXd(row, col);
  for (int i = 0; i < matrix.cols(); i++) {
    for (int j = 0; j < matrix.rows(); j++) {
      matrix(i, j) = mc_random_.uniform_p();
    }
  }
  return matrix;
}

void hmm::init_random() {
  initial_p_ = random_vector(states_size_);
  transition_p_ = random_matrix(states_size_, states_size_);
  emission_p_ = random_matrix(states_size_, alphabet_size_);
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
Eigen::MatrixXd forward_procedure(const std::vector<int> &observation)
{
  
}

Eigen::MatrixXd backward_procedure(const std::vector<int> &observation)
{
  
}
double step(const int &i, const int &j, const int &t)
{
  
}
void hmm::fit(const std::vector<int> &observation,
              const int max_iters)
{
  
}

// Observation explanation: viterbi
std::vector<int> explain(const std::vector<int> &observation)
{
  
}
