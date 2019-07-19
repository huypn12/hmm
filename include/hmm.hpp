#ifndef __HMM_HPP__
#define __HMM_HPP__


#include "dtmc.hpp"

namespace org::mcss {
  class hmm {
  private:
    int hidden_states_size_;
    Eigen::VectorXd initial_p_;
    Eigen::MatrixXd transition_p_;

    int alphabet_size_;
    Eigen::MatrixXd emission_p_;

    static const int MAX_ITERS = 10000;

  protected:
    void forward();
    void backward();
    void pij();



  public:
    hmm();

    /*
      Likelihood estimation: forward-backward algorithm

     */
    double likelihood(const std::vector<int> &observation,
                      const std::vector<int> &hidden_trace);

    // Parameter estimation: baum-welch
    void fit(const std::vector<int> &observation,
             const int max_iters = MAX_ITERS);

    // Observation explanation: viterbi
    std::vector<int> explain(const std::vector<int> &observation); 

    // simulate an observation
    void simulate(const int steps_count);

    
  };
}

#endif
