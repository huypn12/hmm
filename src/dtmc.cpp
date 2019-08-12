#include "dtmc.hpp"

using namespace org::mcss;

dtmc::dtmc(int states_size,
           const Eigen::MatrixXd& p, const Eigen::VectorXd& p0)
  : states_size_(states_size),
    transition_p_(p),
    initial_p_(p0)
{}

int dtmc::transition()
{
  auto pij = transition_p_.row(current_state_);
  return mc_random_.choose_dirichlet(pij);
}

int dtmc::next_state()
{
  auto next_state = transition();
  previous_state_ = current_state_;
  current_state_ = next_state;
  return current_state_;
}

