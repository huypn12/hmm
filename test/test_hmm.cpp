#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "hmm.hpp"

#include <memory>


class fwd_bwd_fixture {
private:
  int n_state_;
  int n_alphabet_;
  Eigen::VectorXd init_;
  Eigen::MatrixXd trans_;
  Eigen::MatrixXd emit_;
  void init_test_data() {
    n_state_ = 3;
    n_alphabet_ = 3;
    init_ << 0.6, 0.4, 0;
    trans_ << 0.69, 0.3, 0.01, 0.4, 0.59, 0.01, 0, 0, 1.0;
    emit_ << 0.5, 0.4, 0.1, 0.1, 0.3, 0.6, 1.0, 0.0, 0.0;
  }

protected:
  std::shared_ptr<org::mcss::hmm> hmm_;

public:
  fwd_bwd_fixture() {
    hmm_ = std::make_shared<org::mcss::hmm>(n_state_, n_alphabet_, init_,
                                            trans_, emit_);
    init_test_data();
  }
};

TEST_CASE_METHOD(fwd_bwd_fixture, "Test Forward Backward", "") {
  std::vector<int> observation({0, 1, 2});
  Eigen::MatrixXd expected(2, 3);
  expected << 0.007518, 0.028120319999999997, 0.01,
    0.01, 0.2109527048413057, 0.7890472951586943;
  auto posterior_marginals = hmm_->smooth(observation);
  REQUIRE(posterior_marginals.isApprox(expected));
}

class baum_welch_fixture {
private:
  int n_state_;
  int n_alphabet_;
  Eigen::VectorXd init_;
  Eigen::MatrixXd trans_;
  Eigen::MatrixXd emit_;

  void init_test_data() {
    n_state_ = 3;
    n_alphabet_ = 3;
    init_ << 0.6, 0.4, 0;
    trans_ << 0.69, 0.3, 0.01, 0.4, 0.59, 0.01, 0, 0, 1.0;
    emit_ << 0.5, 0.4, 0.1, 0.1, 0.3, 0.6, 1.0, 0.0, 0.0;
  }

protected:
  org::mcss::hmm hmm_;

public:
  baum_welch_fixture() : hmm_(n_state_, n_alphabet_, init_, trans_, emit_) {}
};

class viterbi_fixture {
private:
  int n_state_;
  int n_alphabet_;
  Eigen::VectorXd init_;
  Eigen::MatrixXd trans_;
  Eigen::MatrixXd emit_;

  void init_test_data() {
    n_state_ = 3;
    n_alphabet_ = 3;
    init_ << 0.6, 0.4, 0;
    trans_ << 0.69, 0.3, 0.01, 0.4, 0.59, 0.01, 0, 0, 1.0;
    emit_ << 0.5, 0.4, 0.1, 0.1, 0.3, 0.6, 1.0, 0.0, 0.0;
  }

protected:
  org::mcss::hmm hmm_;

public:
  viterbi_fixture() : hmm_(n_state_, n_alphabet_, init_, trans_, emit_) {}
};
