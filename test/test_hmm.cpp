#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "hmm.hpp"

#include <memory>

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
    ss << hmm->next_observation();
  }

  REQUIRE(ss.str().compare("0101010101") == 0);
}

TEST_CASE("Posterior marginals", "") {
  int n_state = 2;
  int n_alphabet = 2;

  auto init = Eigen::VectorXd(n_state);
  auto trans = Eigen::MatrixXd(n_state, n_state);
  auto emit = Eigen::MatrixXd(n_state, n_alphabet);

  init << 0.5, 0.5;
  trans << 0.7, 0.3, 0.3, 0.7;
  emit << 0.9, 0.1, 0.2, 0.8;

  auto hmm =
      std::make_unique<org::mcss::hmm>(n_state, n_alphabet, init, trans, emit);

  std::vector<int> observation({0, 0, 1, 0, 0});

  SECTION("Forward Procedure", "") {
    Eigen::MatrixXd expected(n_state, observation.size() + 1);
    expected <<
      0.5000, 0.8182, 0.8834, 0.1907, 0.7308, 0.8673,
      0.5000, 0.1818, 0.1166, 0.8093, 0.2692, 0.1327;
    auto alpha = hmm->forward(observation);

    std::stringstream ss;
    ss << "Actual: \n" << alpha << std::endl
       << "Expected: \n" << expected << std::endl;
    INFO(ss.str());

    REQUIRE(alpha.cols() == expected.cols());
    REQUIRE(alpha.rows() == expected.rows());
    REQUIRE(alpha.isApprox(expected, 0.0001));
  }

  SECTION("Backward Procedure", "") {
    Eigen::MatrixXd expected(n_state, observation.size() + 1);
    expected <<
      0.6469, 0.5923, 0.3763, 0.6533, 0.6273, 0.5000,
      0.3531, 0.4077, 0.6237, 0.3467, 0.3727, 0.5000;
    auto beta = hmm->backward(observation);

    std::stringstream ss;
    ss << "Backward: \n" << beta << std::endl
       << "Expected: \n" << expected << std::endl;
    INFO(ss.str());

    REQUIRE(beta.cols() == expected.cols());
    REQUIRE(beta.rows() == expected.rows());
    REQUIRE(beta.isApprox(expected, 0.0001));
  }

  SECTION("Posterior", "") {
    Eigen::MatrixXd expected(n_state, observation.size() + 1);
    expected << 0.6469, 0.8673, 0.8204, 0.3075, 0.8204, 0.8673, 0.3531, 0.1327,
        0.1796, 0.6925, 0.1796, 0.1327;
    auto posterior_marginals = hmm->posterior(observation);

    std::stringstream ss;
    ss << "Actual Posterior: \n"
       << posterior_marginals << std::endl
       << "Expected Posterior: \n"
       << expected << std::endl;
    INFO(ss.str());

    REQUIRE(posterior_marginals.cols() == expected.cols());
    REQUIRE(posterior_marginals.rows() == expected.rows());
    REQUIRE(posterior_marginals.isApprox(expected, 0.0001));
  }
}

TEST_CASE("Parameter estimation", "") {
  int n_state = 2;
  int n_alphabet = 2;

  auto init = Eigen::VectorXd(n_state);
  auto trans = Eigen::MatrixXd(n_state, n_state);
  auto emit = Eigen::MatrixXd(n_state, n_alphabet);
  init << 0.5, 0.5;
  trans << 0.95, 0.5, 0.3, 0.7;
  emit << 0.3, 0.7, 0.8, 0.2;

  auto hmm =
      std::make_unique<org::mcss::hmm>(n_state, n_alphabet, init, trans, emit);

  auto new_init = Eigen::VectorXd(n_state);
  auto new_trans = Eigen::MatrixXd(n_state, n_state);
  auto new_emit = Eigen::MatrixXd(n_state, n_alphabet);

  std::vector<int> observation({0, 0, 1, 0, 0});

  hmm->fit(observation, 100);

  new_init << 0.2, 0.8;
  new_trans << 0.5, 0.5, 0.3, 0.7 ;
  new_emit << 0.3, 0.7, 0.8, 0.2;;
}
