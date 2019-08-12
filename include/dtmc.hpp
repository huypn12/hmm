#ifndef __DTMC_HPP__
#define __DTMC_HPP__

#include "mc_random.hpp"
#include "markov.hpp"

namespace org::mcss {
  class dtmc {
  protected:
    mc_random mc_random_;

    int current_state_;
    int previous_state_;
    int states_size_;
    Eigen::VectorXd initial_p_;
    Eigen::MatrixXd transition_p_;

    int transition();

   public:
    dtmc(int state_size, const Eigen::MatrixXd &p, const Eigen::VectorXd &p0);

    // markov trace stream
    int next_state();
    int get_current_state();
    int get_previous_state();
  };

} // namespace org::mcss

#endif
