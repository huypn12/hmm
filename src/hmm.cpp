#include "hmm.hpp"
#include "mc_random.hpp"

using namespace org::mcss;


void populate(Eigen::MatrixXd &matrix) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  for (int i = 0; i < matrix.cols(); i++) {
    for (int j = 0; j < matrix.rows(); j++) {
      matrix(i, j) = distribution(generator);
    }
  }
}


Eigen::MatrixXd random_matrix(const int &row, const int &col){
  Eigen::MatrixXd matrix = Eigen::MatrixXd();
  populate(matrix);
  return matrix;
}


hmm::hmm(const int &hidden_states_card, const int &alphabet_card) {
  hidden_states_card_ = hidden_states_card;
  alphabet_card_ = alphabet_card;

  initial_p_ = random_matrix(1, hidden_states_card_);
  transition_p_ = random_matrix(hidden_states_card_, hidden_states_card_);
  emission_p_ = random_matrix(hidden_states_card_, alphabet_card_);
}

double hmm::pr_ij_t(const int &i, const int &j, const int &t)
{
  return 0.0;
}
double hmm::likelihood(const std::vector<int> &observation, const std::vector<int> &hidden_trace) {
  return 0;
}
std::vector<int> hmm::next_observation() {
  return std::vector<int>();
}

std::vector<int> hmm::simulate(const int steps_count)
{
  mc_random rand_engine;
  std::vector<int> trace;
  for (int i = 0; i < steps_count; i++) {
    int hidden_state = rand_engine.choose_dirichlet(transition_p_.col(i));
    int observation = rand_engine.choose_dirichlet(emission_p_.col(hidden_state));
  }
  return trace;
}


