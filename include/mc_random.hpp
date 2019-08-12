#ifndef __MC_RANDOM_HPP__
#define __MC_RANDOM_HPP__

#include <random>
#include <Eigen/Eigen>

namespace org::mcss {
  class mc_random {
  private:
    int seed_;
    std::mt19937_64 generator_;

  protected:
    void init(int seed);

  public:
    mc_random();
    mc_random(int seed);

    void reset();

    int choose_uniform(const int &n_states);
    int choose_dirichlet(const std::vector<double> &distribution);
    int choose_dirichlet(const Eigen::VectorXd &distribution);

    double uniform_p();
  };

} // org::mcss

#endif //HMM_INCLUDE_MC_RANDOM_HPP
