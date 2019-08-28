#ifndef __HMM_HPP__
#define __HMM_HPP__

#include "dtmc.hpp"

#include <string>
#include <vector>

namespace org::mcss {
class hmm : protected dtmc {
private:
protected:
  int alphabet_size_;
  Eigen::MatrixXd emission_p_;
  static const int BW_MAXITERS = 100;

  double log_likelihood_ = 0;
  double aic_ = 0;

  Eigen::MatrixXd xi_t(int t, const Eigen::MatrixXd &alpha, const Eigen::MatrixXd &beta);
  void expectation(const std::vector<int> &observation, Eigen::MatrixXd &gamma,
                   Eigen::MatrixXd &p);
  double maximization(const std::vector<int> &observation,
                    const Eigen::MatrixXd &gamma,
                    const Eigen::MatrixXd &p,
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
  Eigen::MatrixXd posterior(const std::vector<int> &observation,
                            const Eigen::MatrixXd &fwd,
                            const Eigen::MatrixXd &bwd);
  Eigen::MatrixXd posterior(const std::vector<int> &observation);

  // Parameter estimation: baum-welch
  void fit(const std::vector<int> &observation,
           const int max_iters = BW_MAXITERS, const double eps = 1e-6);

  // Observation explanation: viterbi
  std::vector<int> decode(const std::vector<int> &observation);
};
} // namespace org::mcss

#endif
