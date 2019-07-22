#ifndef __MC_RANDOM_HPP__
#define __MC_RANDOM_HPP__

#include <random>
#include <eigen3/Eigen/Dense>

namespace org::mcss {

class mc_random {
private:
  std::default_random_engine generator_;

protected:
  void init(int seed);

public:
  int choose_uniform(const int &n_states);
  int choose_dirichlet(const Eigen::VectorXd &distribution);
};

}

#endif //HMM_INCLUDE_MC_RANDOM_HPP
