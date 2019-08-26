#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "hmm.hpp"

#include <memory>



class fwd_bwd_fixture {
protected:
  std::unique_ptr<org::mcss::hmm> hmm_;

public:
  fwd_bwd_fixture(){
    int n_state = 2;
    int n_alphabet = 2;

    auto init = Eigen::VectorXd(n_state);
    auto trans = Eigen::MatrixXd(n_state, n_state);
    auto emit = Eigen::MatrixXd(n_state, n_alphabet);

    init << 0.5, 0.5;
    trans << 0.7, 0.3, 0.3, 0.7;
    emit << 0.9, 0.1, 0.2, 0.8;

   hmm_ = std::make_unique<org::mcss::hmm>(n_state, n_alphabet, init,
                                            trans, emit);
  }
};

TEST_CASE_METHOD(fwd_bwd_fixture, "Test simulation", "[non-assert]") {
  std::stringstream ss;
  for (int i = 0; i < 100; i++) {
    ss << hmm_->next_observation() << " ";
  }
  ss << std::endl;
  INFO(ss.str());
}

TEST_CASE_METHOD(fwd_bwd_fixture, "Test info", "[non-assert]") {
  INFO(hmm_->model_info());
}

TEST_CASE_METHOD(fwd_bwd_fixture, "Test Forward Backward", "[smooth]") {
  std::vector<int> observation({0, 0, 1, 0, 0});
  Eigen::MatrixXd expected(2, 6);
  expected << 0.6469, 0.8673, 0.8204, 0.3075, 0.8204, 0.8673,
      0.3531, 0.1327, 0.1796, 0.6925, 0.1796, 0.1327;

  std::stringstream ss;
  auto alpha = hmm_->forward(observation);
  ss << "Forward: \n" << alpha << std::endl;
  auto beta = hmm_->backward(observation);
  ss << "Backward: \n" << beta << std::endl;
  auto posterior_marginals = hmm_->posterior(observation);
  ss << "Actual Posterior: \n" << posterior_marginals << std::endl;
  ss << "Expected Posterior: \n" << expected << std::endl;

  INFO(ss.str());
  REQUIRE(posterior_marginals.isApprox(expected, 0.0001));
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
