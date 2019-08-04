#ifndef __HMM_HPP__
#define __HMM_HPP__

#include "dtmc.hpp"

namespace org::mcss {
class hmm : public dtmc {
 private:

  int alphabet_card_;
  Eigen::MatrixXd emission_p_;

  static const int BW_MAXITERS = 10000;

 protected:
  /*
    Forward procedure
    @param
    @param
   */
  Eigen::MatrixXd forward(const std::vector<int> &observation);

  /*
    Backward procedure
   */
  Eigen::MatrixXd backward(const std::vector<int> &observation);
  /*
    Transition probability from state i to state j at time t
    Pr[s_(t+1)=j, s_(t)=i]
    @param i state at time t
    @param j state at time t+1
    @param t timestep
   */
  double pr_ij_t(const int &i, const int &j, const int &t);

 public:
  hmm(const int &hidden_states_card, const int &alphabet_card);
  // Likelihood estimation: forward-backward algorithm
  double likelihood(const std::vector<int> &observation,
                    const std::vector<int> &hidden_trace);

  // Parameter estimation: baum-welch
  void fit(const std::vector<int> &observation,
           const int max_iters = BW_MAXITERS);

  // Observation explanation: viterbi
  std::vector<int> explain(const std::vector<int> &observation);

  // simulate an observation
  std::vector<int> next_observation();
};
}

#endif
