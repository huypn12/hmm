#include "dtmc.hpp"


using namespace org::mcss;

dtmc::dtmc(const Eigen::MatrixXf& p, const Eigen::VectorXf& p0)
{
  trans_p_ = p;
  initial_p_ = p0;
}

int dtmc::jump()
{
  
}

int dtmc::next_state()
{
  return jump();
}

