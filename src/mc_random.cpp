#include "mc_random.hpp"

using namespace org::mcss;

int mc_random::choose_uniform(const int &n_states)
{
  std::uniform_int_distribution<int> distribution(0, n_states - 1);
  return distribution(generator_);
}

int mc_random::choose_dirichlet(const std::vector<double> &distribution_)
{
  std::discrete_distribution<int> distribution(distribution_.begin(),
                                               distribution_.end());
  return distribution(generator_);
}
