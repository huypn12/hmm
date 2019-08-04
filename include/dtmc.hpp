#ifndef __DTMC_HPP__
#define __DTMC_HPP__

#include "markov.hpp"

#include "eigen3/Eigen/Dense"

#include <random>

namespace org::mcss {

class dtmc : public markov {
 private:
  int hidden_states_size_;
  Eigen::MatrixXd initial_p_;
  Eigen::MatrixXd transition_p_;

  int current_state_;

 public:
  dtmc();
  dtmc(const Eigen::MatrixXf &p, const Eigen::VectorXf &p0);

  // stream
  int next_state();
  int current_state();
};

} // namespace org::mcss

#endif
