#ifndef __HMM_HPP__
#define __HMM_HPP__

#include "dtmc.hpp"
#include <vector>

namespace org::mcss {
class hmm : public dtmc {
private:
  Eigen::MatrixXd random_matrix(const int &row, const int &col);
  Eigen::VectorXd random_vector(const int &dim);

protected:
  int alphabet_size_;
  Eigen::MatrixXd emission_p_;
  static const int BW_MAXITERS = 10000;

  Eigen::MatrixXd forward_procedure(const std::vector<int> &observation);
  Eigen::MatrixXd backward_procedure(const std::vector<int> &observation);
  double step(const int &i, const int &j, const int &t);

public:
  hmm(const int &states_size, const int &alphabet_size,
      const Eigen::VectorXd &p0, const Eigen::MatrixXd &p,
      const Eigen::MatrixXd &b);
  hmm(const int &states_size, const int &alphabet_size);
  void init_random();

  // stream observation
  int next_observation();

  // Likelihood estimation: forward-backward algorithm
  double likelihood(const std::vector<int> &observation,
                    const std::vector<int> &hidden_trace);

  // Parameter estimation: baum-welch
  void fit(const std::vector<int> &observation,
           const int max_iters = BW_MAXITERS);

  // Observation explanation: viterbi
  std::vector<int> explain(const std::vector<int> &observation);
};
} // namespace org::mcss

#endif
