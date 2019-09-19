#ifndef __DTMC_HPP__
#define __DTMC_HPP__

#include "markov.hpp"
#include "mc_random.hpp"

namespace org::mcss {
class dtmc {
protected:
  mc_random mc_random_;

  int current_state_ = -1;
  int previous_state_ = -1;
  int state_count_;
  Eigen::VectorXd initial_p_;
  Eigen::MatrixXd transition_p_;

  int transition();

public:
  dtmc(int state_count);
  dtmc(int state_count, const Eigen::VectorXd &initial_p,
       const Eigen::MatrixXd &transition_p);
  std::string model_info();

  const Eigen::VectorXd &initial_p() { return initial_p_; }
  const Eigen::MatrixXd &transition_p() { return transition_p_; };

  // markov trace stream
  int next_state();
  int get_current_state();
  int get_previous_state();
};

} // namespace org::mcss

#endif
