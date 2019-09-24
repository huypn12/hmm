#ifndef __DTMC_HPP__
#define __DTMC_HPP__

#include "markov.hpp"
#include "mc_random.hpp"

namespace org::mcss {
class dtmc : public markov {
protected:
  mc_random mc_random_;

  int state_count_;
  int current_state_ = begin_state_;
  int previous_state_ = begin_state_;

  Eigen::VectorXd initial_p_;
  Eigen::MatrixXd transition_p_;

  int jump();

public:
  dtmc(int state_count);
  dtmc(int state_count, const Eigen::VectorXd &initial_p,
       const Eigen::MatrixXd &transition_p);
  std::string str();

  const Eigen::VectorXd &initial_p() { return initial_p_; }
  const Eigen::MatrixXd &transition_p() { return transition_p_; };
  const int &current_state() override { return current_state_; };
  const int &previous_state() override { return previous_state_; };

  // markov trace stream
  int next() override;
};

} // namespace org::mcss

#endif
