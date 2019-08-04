#ifndef __DTMC_HPP__
#define __DTMC_HPP__

#include "mc_random.hpp"
#include "markov.hpp"

#include "eigen3/Eigen/Dense"

#include <random>

namespace org::mcss {

class dtmc : public markov {
 protected:
  int current_state_;
  int previous_state_;

  mc_random mc_random_;

  int hidden_states_size_;
  Eigen::MatrixXd initial_p_;
  Eigen::MatrixXd transition_p_;

  int transition();

 public:
  dtmc();
  dtmc(const Eigen::MatrixXf &p, const Eigen::VectorXf &p0);

  // markov stream
  int next_state();
  int get_current_state();
  int get_previous_state();
};
} // namespace org::mcss

#endif
