#include "hmm.hpp"

using namespace org::mcss;

hmm::hmm(const int &states_size, const int &alphabet_count)
    : dtmc(states_size), alphabet_count_(alphabet_count), emission_p_{} {
  emission_p_ = Eigen::MatrixXd::Zero(states_size, alphabet_count);
}

hmm::hmm(const int &state_count, const int &alphabet_count,
         const Eigen::VectorXd &initial_p, const Eigen::MatrixXd &transition_p,
         const Eigen::MatrixXd &emission_p)
    : dtmc(state_count, initial_p, transition_p),
      alphabet_count_(alphabet_count), emission_p_{} {
  emission_p_ = emission_p;
}

std::string hmm::str() {
  std::stringstream ss;
  ss << dtmc::str() << std::endl;
  ss << "Observation cardinality: " << alphabet_count_ << std::endl;
  ss << "Emission probabilities: \n" << emission_p_ << std::endl;

  ss << "* Model inferences: " << std::endl;
  ss << "Log likelihood: " << log_likelihood_ << std::endl;
  ss << "AIC: " << aic_ << std::endl;

  return ss.str();
}

// Simulate trace
int hmm::next() {
  int state = dtmc::next();
  int observation = mc_random_.choose_dirichlet(emission_p_.row(state));
  return observation;
}

void hmm::forward(const std::vector<int> &observation) {
  auto T = observation.size();
  alpha_.resize(state_count_, T);
  alpha_.setZero();
  // basis step
  alpha_.col(0) = initial_p_.cwiseProduct(emission_p_.col(observation[0]));
  alpha_.col(0) = alpha_.col(0) / alpha_.col(0).sum();
  // inductive step
  for (int t = 1; t < T; t++) {
    alpha_.col(t) = alpha_.col(t - 1).transpose() * transition_p_;
    alpha_.col(t) = alpha_.col(t).cwiseProduct(emission_p_.col(observation[t]));
    alpha_.col(t) = alpha_.col(t) / alpha_.col(t).sum();
  }
}

void hmm::backward(const std::vector<int> &observation) {
  auto T = observation.size();
  beta_.resize(state_count_, T);
  beta_.setZero();
  // basis step
  beta_.col(T - 1) =
      Eigen::VectorXd(state_count_).setConstant(1 / state_count_);
  // inductive step
  for (int t = T - 2; t >= 0; t--) {
    beta_.col(t) =
        beta_.col(t + 1).cwiseProduct(emission_p_.col(observation[t + 1]));
    beta_.col(t) = (transition_p_ * beta_.col(t)).transpose();
    auto sum = beta_.col(t).cwiseProduct(emission_p_.col(observation[t])).sum();
    beta_.col(t) = beta_.col(t) / sum;
  }
}

const Eigen::MatrixXd &hmm::posterior(const std::vector<int> &observation) {
  auto T = observation.size();
  gamma_ = Eigen::MatrixXd(state_count_, T);
  forward(observation);
  backward(observation);
  for (int t = 0; t < T; t++) {
    gamma_.col(t) = alpha_.col(t).cwiseProduct(beta_.col(t));
    gamma_.col(t) = gamma_.col(t) / gamma_.col(t).sum();
  }
  return gamma_;
}

void hmm::init_random() {
  initial_p_ = mc_random_.random_vector(state_count_);
  transition_p_ = mc_random_.random_matrix(state_count_, state_count_);
  emission_p_ = mc_random_.random_matrix(state_count_, alphabet_count_);
}

void hmm::expectation(const std::vector<int> &observation) {
  auto T = observation.size();
  posterior(observation);
  sigma_xi_.resize(state_count_, state_count_);
  sigma_xi_.setZero();
  for (int t = 0; t < T - 1; t++) {
    auto xi = Eigen::MatrixXd(transition_p_);
    xi = xi.array().colwise() * alpha_.col(t).array();
    xi = xi.array().colwise() * emission_p_.col(observation[t + 1]).array();
    xi = xi.array().rowwise() * beta_.col(t + 1).transpose().array();
    xi = xi / xi.sum();
    sigma_xi_ += xi;
  }
  // TODO: asssert log likelihood is the same among all columns
  log_likelihood_ = log(alpha_.col(T).sum());
  auto param_count = state_count_ * state_count_ +
                     state_count_ * alphabet_count_ + state_count_;
  aic_ = -2 * log_likelihood_ + 2 * param_count;
}

double hmm::maximization(const std::vector<int> &observation) {
  auto T = observation.size();

  Eigen::VectorXd new_initial = gamma_.rowwise().sum() / observation.size();
  Eigen::MatrixXd new_transition =
      sigma_xi_.array().colwise() / gamma_.rowwise().sum().array();
  Eigen::MatrixXd new_emission =
      Eigen::MatrixXd::Zero(state_count_, alphabet_count_);
  for (int t = 0; t < T; t++) {
    auto o = observation[t];
    new_emission.col(o) += gamma_.col(t);
  }
  new_emission = new_emission.array().colwise() / gamma_.rowwise().sum().array();

  auto norm_diff = 0.0;
  norm_diff += (new_initial - initial_p_).norm();
  norm_diff += (new_transition - transition_p_).norm();
  norm_diff += (new_emission - emission_p_).norm();

  initial_p_ = new_initial;
  transition_p_ = new_transition;
  emission_p_ = new_emission;

  return norm_diff;
}

void hmm::fit(const std::vector<int> &observation, const int &max_iters,
              const double &eps) {
  auto T = observation.size();
  for (int i = 0; i < max_iters; i++) {
    expectation(observation);
    auto norm_diff = maximization(observation);
    if (norm_diff <= eps) {
      break;
    }
    last_iter_ = i;
  }
}

// Observation explanation: viterbi
std::vector<int> decode(const std::vector<int> &observation) {
  // TODO: implement viterbi algorithm
  return observation;
}
