#ifndef __HMM_HPP__
#define __HMM_HPP__

#include "dtmc.hpp"

namespace org::mcss {
class hmm : public dtmc {
protected:
  int alphabet_count_;
  Eigen::MatrixXd emission_p_;

  int current_obs_;
  int previous_obs_;

  static const int k_max_iters_ = 1000;
  static constexpr double k_eps_ = 1e-9;
  double log_likelihood_ = 0;
  double aic_ = 0;
  int last_iter_ = 0;

  Eigen::MatrixXd alpha_;
  Eigen::MatrixXd beta_;
  Eigen::MatrixXd gamma_;
  Eigen::MatrixXd sigma_xi_;
  void forward(const std::vector<int> &observation);
  void backward(const std::vector<int> &observation);
  void expectation(const std::vector<int> &observation);
  double maximization(const std::vector<int> &observation);

public:
  hmm(const int &states_size, const int &alphabet_count,
      const Eigen::VectorXd &p0, const Eigen::MatrixXd &p,
      const Eigen::MatrixXd &b);
  hmm(const int &states_size, const int &alphabet_count);

  // info
  std::string str();

  // getter
  const Eigen::MatrixXd &emission_p() { return emission_p_; };

  // stream observation
  const int &current_obs() { return current_obs_; };
  const int &previous_obs() { return previous_obs_; };
  int next();

  // init model parameters
  void init_random();

  // Likelihood estimation: forward-backward algorithm
  const Eigen::MatrixXd &alpha() { return alpha_; }
  const Eigen::MatrixXd &beta() { return alpha_; }
  const Eigen::MatrixXd &gamma() { return gamma_; }
  const Eigen::MatrixXd &posterior(const std::vector<int> &observation);

  // Parameter estimation: baum-welch
  const double &log_likelihood() { return log_likelihood_; }
  const double &aic() { return aic_; }
  const int &last_iter() { return last_iter_; };
  void fit(const std::vector<int> &observation,
           const int &max_iters = k_max_iters_, const double &eps = k_eps_);

  // Observation explanation: viterbi
  std::vector<int> decode(const std::vector<int> &observation);
};
} // namespace org::mcss

#endif
