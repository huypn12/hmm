#include "mc_random.hpp"
#include <vector>


using namespace org::mcss;

void McRandom::reset()
{
  generator_ = std::mt19937_64(seed_);
}

McRandom::McRandom(int seed)
  : seed_(seed),
    generator_(std::mt19937_64(seed))
{
}

McRandom::McRandom() {
  std::random_device rd;
  seed_ = rd();
  generator_ = std::mt19937_64(seed_);
}

int McRandom::ChooseUniform(const int &n_states)
{
  std::uniform_int_distribution<int> distribution(0, n_states - 1);
  return distribution(generator_);
}

int McRandom::ChooseDirichlet(const std::vector<double> &p) {
  std::discrete_distribution<int> distribution(p.begin(), p.end());
  return distribution(generator_);
}

int McRandom::ChooseDirichlet(const Eigen::VectorXd &p)
{
  std::vector<double> v_dist;
  v_dist.resize(p.size());
  Eigen::VectorXd::Map(&v_dist[0], v_dist.size()) = p;
  return ChooseDirichlet(v_dist);
}

double McRandom::RandomProbUniform()
{
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  return distribution(generator_);
}

Eigen::VectorXd McRandom::RandomStochasticVector(const int &dim) {
  auto v = Eigen::VectorXd(dim);
  for (int i = 0; i < v.size(); i++) {
    v(i) = RandomProbUniform();
  }
  v = v / v.sum();
  return v;
}

Eigen::MatrixXd McRandom::RandomStochasticMatrix(const int &row, const int &col) {
  auto matrix = Eigen::MatrixXd(row, col);
  for (int j = 0; j < matrix.rows(); j++) {
    matrix.row(j) = RandomStochasticVector(col);
  }
  return matrix;
}
