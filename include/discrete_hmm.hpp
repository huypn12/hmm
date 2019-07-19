#ifndef __HMM_HPP__
#define __HMM_HPP__

#include "dtmc.hpp"

namespace org::mcss {
  // Hidden Markov Model of discrete observations
  class discrete_hmm {
  private:
    dtmc hidden_dtmc;
    Eigen::MatrixXf emission_p_;

  protected:

  public:
    int next_observation();
};

} // namespace org::mcss

#endif
