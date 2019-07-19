#ifndef __DTMC_HPP__
#define __DTMC_HPP__

#include "markov.hpp"

#include "eigen3/Eigen/Dense"

#include <random>

namespace org::mcss {

  class dtmc: public markov {
  private:
    Eigen::MatrixXf trans_p_;
    Eigen::VectorXf initial_p_;

    int current_state_;
    int jump();

  public:
    dtmc();
    dtmc(const Eigen::MatrixXf &p, const Eigen::VectorXf &p0);

    int next_state();
    int current_state();
};

} // namespace org::mcss

#endif
