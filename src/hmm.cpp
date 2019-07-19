#include "hmm.hpp"

using namespace org::mcss;

hmm::hmm() {
  initial_p_ = Eigen::VectorXd(hidden_states_size_);
  transition_p_ = Eigen::MatrixXd(hidden_states_size_, hidden_states_size_);
  emission_p_ = Eigen::MatrixXd(hidden_states_size_, alphabet_size_);

  /*
    define populate(matrix/vector A, bool random=true)
    populate a matrix with value of u.a.r selected in [0, 1]
   */
  populate(initial_p_);
  populate(transition_p_);
  populate(emission_p_);
}


