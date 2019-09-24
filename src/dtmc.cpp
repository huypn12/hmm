#include "dtmc.hpp"

using namespace org::mcss;

dtmc::dtmc(int state_count, const Eigen::VectorXd &initial_p,
           const Eigen::MatrixXd &transition_p)
    : state_count_(state_count), initial_p_{}, transition_p_{} {
  initial_p_ = initial_p;
  transition_p_ = transition_p;
}

dtmc::dtmc(int state_count)
    : state_count_(state_count), initial_p_{}, transition_p_{} {
  initial_p_ = Eigen::VectorXd::Zero(state_count);
  transition_p_ = Eigen::MatrixXd::Zero(state_count, state_count);
}

int dtmc::jump() {
  if (current_state_ == begin_state_) {
    return mc_random_.choose_dirichlet(initial_p_);
  }
  auto pij = transition_p_.row(current_state_);
  return mc_random_.choose_dirichlet(pij);
}

int dtmc::next() {
  auto next_state = jump();
  previous_state_ = current_state_;
  current_state_ = next_state;
  return current_state_;
}

std::string dtmc::str() {
  std::stringstream ss;
  ss << "Initial distribution: \n"
     << initial_p_ << std::endl
     << "Transition matrix: \n"
     << transition_p_ << std::endl;
  return ss.str();
}
