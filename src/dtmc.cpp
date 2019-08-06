#include "dtmc.hpp"


using namespace org::mcss;

dtmc::dtmc(const Eigen::MatrixXf& p, const Eigen::VectorXf& p0)
{
  transition_p_ = p;
  initial_p_ = p0;
}

int dtmc::transition()
{
  auto step_p = transition_p_.row(current_state_);
  std::vector<double> v2;
  v2.resize(step_p.size());
  return mc_random_.choose_dirichlet();
}

int dtmc::next_state()
{
  auto next_state = transition();
  previous_state_ = current_state_;
  current_state_ = next_state;
  return current_state_;
}

