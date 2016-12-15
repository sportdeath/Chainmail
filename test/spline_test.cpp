#include <sweater/Splines/BSplineUnit.hpp>
#include <sweater/Splines/polyvector3f.hpp>
#include <sweater/Splines/polynomial.hpp>
#include <sweater/Objects/yarnUnit.hpp>

#include "gtest/gtest.h"

TEST(SplineTest, Polynomials) {
  std::vector<Eigen::Vector3f> points = {
    Eigen::Vector3f(-2,3,-7),
    Eigen::Vector3f(1,-2,1),
    Eigen::Vector3f(-4,6,2),
    Eigen::Vector3f(-5,3,1),
  };
  BSplineUnit spline(points.begin());

  PolyVector3f polys = spline.getPolynomials();

  ASSERT_EQ(polys[0],Polynomial({-1/3., -1, -4, 2}));

  ASSERT_NEAR(polys[1].getCoefficient(0), 1/6., 0.0001);
  ASSERT_NEAR(polys[1].getCoefficient(1), 3/2., 0.0001);
  ASSERT_NEAR(polys[1].getCoefficient(2), 13/2., 0.0001);
  ASSERT_NEAR(polys[1].getCoefficient(3), -4., 0.0001);

  ASSERT_NEAR(polys[2].getCoefficient(0), -1/6., 0.0001);
  ASSERT_NEAR(polys[2].getCoefficient(1), 9/2., 0.0001);
  ASSERT_NEAR(polys[2].getCoefficient(2), -7/2., 0.0001);
  ASSERT_NEAR(polys[2].getCoefficient(3), 5/6., 0.0001);
}

TEST(SplineTest, PolynomialsP) {
  std::vector<Eigen::Vector3f> points = {
    Eigen::Vector3f(-2,3,-7),
    Eigen::Vector3f(1,-2,1),
    Eigen::Vector3f(-4,6,2),
    Eigen::Vector3f(-5,3,1),
  };
  BSplineUnit spline(points.begin());

  PolyVector3f polys = spline.getPolynomialsP();

  ASSERT_EQ(polys[0],Polynomial({-1., -8, 6}));

  ASSERT_NEAR(polys[1].getCoefficient(0), 3/2., 0.0001);
  ASSERT_NEAR(polys[1].getCoefficient(1), 13., 0.0001);
  ASSERT_NEAR(polys[1].getCoefficient(2), -12., 0.0001);
  ASSERT_EQ(polys[1].getSize(), 3);

  ASSERT_NEAR(polys[2].getCoefficient(0), 9/2., 0.0001);
  ASSERT_NEAR(polys[2].getCoefficient(1), -7, 0.0001);
  ASSERT_NEAR(polys[2].getCoefficient(2), 5/2., 0.0001);
  ASSERT_EQ(polys[2].getSize(), 3);
}

TEST(SplineTest, PolynomialsPP) {
  std::vector<Eigen::Vector3f> points = {
    Eigen::Vector3f(-2,3,-7),
    Eigen::Vector3f(1,-2,1),
    Eigen::Vector3f(-4,6,2),
    Eigen::Vector3f(-5,3,1),
  };
  BSplineUnit spline(points.begin());

  PolyVector3f polys = spline.getPolynomialsPP();

  ASSERT_EQ(polys[0],Polynomial({-8., 12.}));
  ASSERT_EQ(polys[1],Polynomial({13., -24}));
  ASSERT_EQ(polys[2],Polynomial({-7., 5}));
}

TEST(SplineTest, yarnUnit) {
  std::vector<Eigen::Vector3f> points = {
    Eigen::Vector3f(-2,3,-7),
    Eigen::Vector3f(1,-2,1),
    Eigen::Vector3f(-4,6,2),
    Eigen::Vector3f(-5,3,1),
  };


  YarnUnit yarnUnit(points.begin(), 10, NULL);

  Ray ray(
      Eigen::Vector3f(0,0,-9),
      Eigen::Vector3f(0,0,1));
  Hit hit;

  yarnUnit.intersect(ray, 0, hit);
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
