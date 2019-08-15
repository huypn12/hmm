#include "mc_random.hpp"


using namespace org::mcss;

void mc_random::init(int seed)
{
  seed_ = seed;
  reset();
}

void mc_random::reset()
{
  generator_ = std::mt19937_64(seed_);
}

mc_random::mc_random(int seed)
{
  init(seed);
}

mc_random::mc_random() {
  std::random_device rd;
  generator_ = std::mt19937_64(rd());
}

int mc_random::choose_uniform(const int &n_states)
{
  std::uniform_int_distribution<int> distribution(0, n_states - 1);
  return distribution(generator_);
}

int mc_random::choose_dirichlet(const std::vector<double> &p) {
  std::discrete_distribution<int> distribution(p.begin(), p.end());
  return distribution(generator_);
}

int mc_random::choose_dirichlet(const Eigen::VectorXd &p)
{
  std::vector<double> v_dist;
  v_dist.resize(p.size());
  Eigen::VectorXd::Map(&v_dist[0], v_dist.size()) = p;
  return choose_dirichlet(v_dist);
}

double mc_random::uniform_p()
{
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  return distribution(generator_);
}
Eigen::VectorXd mc_random::random_vector(const int &dim) {
  auto vector = Eigen::VectorXd(dim);
  for (int i = 0; i < vector.size(); i++) {
    vector(i) = uniform_p();
  }
  return vector;
}

Eigen::MatrixXd mc_random::random_matrix(const int &row, const int &col) {
  auto matrix = Eigen::MatrixXd(row, col);
  for (int i = 0; i < matrix.cols(); i++) {
    for (int j = 0; j < matrix.rows(); j++) {
      matrix(i, j) = uniform_p();
    }
  }
  return matrix;
}
