#define CATCH_CONFIG_MAIN

#include "hmm.hpp"

#include <catch.hpp>

#include <memory>

using namespace org::mcss;

TEST_CASE("Simple trace simulation", "") {
  int n_state = 2;
  int n_alphabet = 2;

  auto init = Eigen::VectorXd(n_state);
  auto trans = Eigen::MatrixXd(n_state, n_state);
  auto emit = Eigen::MatrixXd(n_state, n_alphabet);

  init << 1, 0;
  trans << 0, 1, 1, 0;
  emit << 1, 0, 0, 1;

  auto hmm =
      std::make_unique<org::mcss::hmm>(n_state, n_alphabet, init, trans, emit);

  std::stringstream ss;
  for (int i = 0; i < 10; i++) {
    ss << hmm->next();
  }

  REQUIRE(ss.str().compare("0101010101") == 0);
}

TEST_CASE("Posterior marginals, Forward-Backward", "") {
  int n_state = 2;
  int n_alphabet = 3;

  auto init = Eigen::VectorXd(n_state);
  init << 1.0, 0.0;
  auto trans = Eigen::MatrixXd(n_state, n_state);
  trans.setConstant(1.0 / 2);
  auto emit = Eigen::MatrixXd(n_state, n_alphabet);
  emit.setConstant(1.0 / 3);
  auto model = std::make_unique<hmm>(n_state, n_alphabet, init, trans, emit);

  SECTION("Forward Procedure", "") {}

  SECTION("Backward Procedure", "") {}

  SECTION("Posterior", "") {}
}
