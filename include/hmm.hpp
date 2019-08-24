#ifndef __HMM_HPP__
#define __HMM_HPP__

#include "dtmc.hpp"

#include <vector>
#include <string>

namespace org::mcss {
class hmm : public dtmc {
private:
protected:
  int alphabet_size_;
  Eigen::MatrixXd emission_p_;
  static const int BW_MAXITERS = 10000;

  double log_likelihood_ = 0;
  double aic_ = 0;

  void expectation(Eigen::MatrixXd &gamma, Eigen::MatrixXd &p);
  void maximization(const Eigen::MatrixXd &gamma, const Eigen::MatrixXd &p,
                    Eigen::VectorXd &new_initial,
                    Eigen::MatrixXd &new_transition,
                    Eigen::MatrixXd &new_emission);

public:
  hmm(const int &states_size, const int &alphabet_size,
      const Eigen::VectorXd &p0, const Eigen::MatrixXd &p,
      const Eigen::MatrixXd &b);
  hmm(const int &states_size, const int &alphabet_size);
  void init_random();

  // stream observation
  int next_observation();

  // info
  std::string model_info();

  // Likelihood estimation: forward-backward algorithm
  Eigen::MatrixXd forward(const std::vector<int> &observation);
  Eigen::MatrixXd backward(const std::vector<int> &observation);
  Eigen::MatrixXd posterior(const std::vector<int> &observation);

  // Parameter estimation: baum-welch
  void fit(const std::vector<int> &observation,
           const int max_iters = BW_MAXITERS);

  // Observation explanation: viterbi
  std::vector<int> decode(const std::vector<int> &observation);
};
} // namespace org::mcss

#endif
