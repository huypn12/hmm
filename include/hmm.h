#ifndef __HMM_HPP__
#define __HMM_HPP__

#include "dtmc.h"
#include <string>
#include <vector>

namespace org::mcss {
class Hmm {
private:
  MarkovRandom random_;

  Dtmc dtmc_;
  int alphabet_count_;
  Eigen::MatrixXd emission_p_;

  int current_obs_;
  int previous_obs_;

  static const int kMaxIters = 1000;
  static constexpr double kEps = 1e-9;
  Eigen::MatrixXd alpha_;
  Eigen::MatrixXd beta_;
  Eigen::MatrixXd gamma_;
  Eigen::MatrixXd sigma_xi_;
  double log_likelihood_ = 0;
  double aic_ = 0;
  int last_iter_ = 0;

protected:
  double UpdateParams(const Eigen::VectorXd &, const Eigen::MatrixXd &,
                      const Eigen::MatrixXd &);
  void Forward(const std::vector<int> &observation);
  void Backward(const std::vector<int> &observation);
  void Expectation(const std::vector<int> &observation);
  double Maximization(const std::vector<int> &observation);

public:
  Hmm(const int &states_size, const int &alphabet_count,
      const Eigen::VectorXd &p0, const Eigen::MatrixXd &p,
      const Eigen::MatrixXd &b);
  Hmm(const int &states_size, const int &alphabet_count);

  std::string Str();

  // simulation
  int Next();

  // init model parameters
  void InitRandom();

  // Likelihood estimation: forward-backward algorithm
  const Eigen::MatrixXd &Posterior(const std::vector<int> &observation);

  // Parameter estimation: baum-welch
  void Fit(const std::vector<int> &observation,
           const int &max_iters = kMaxIters, const double &eps = kEps);

  // Observation explanation: viterbi
  std::vector<int> Decode(const std::vector<int> &observation);

  // getter
  const int &alphabet_count() { return alphabet_count_; }
  const Eigen::MatrixXd &emission_p() { return emission_p_; };
  void emission_p(const Eigen::MatrixXd& m) { emission_p_ = m; }
  const Eigen::MatrixXd &alpha() { return alpha_; }
  const Eigen::MatrixXd &beta() { return beta_; }
  const Eigen::MatrixXd &gamma() { return gamma_; }
  const double &log_likelihood() { return log_likelihood_; }
  const double &aic() { return aic_; }
  const int &last_iter() { return last_iter_; };
  const int &current_obs() { return current_obs_; };
  const int &previous_obs() { return previous_obs_; };
};
} // namespace org::mcss

#endif
