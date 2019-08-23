//
// Created by huypn12 on 6/11/19.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <Eigen/Eigen>

TEST_CASE("Eigen VectorXd normalization", "[Eigen]") {
  Eigen::VectorXd v(2);
  v << 1.0, 1.0;
  Eigen::VectorXd expect(2);
  expect << 0.5, 0.5;
  v.lpNorm<1>();
  std::cout << v;
  REQUIRE(v.isApprox(expect));
}
