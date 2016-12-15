#include <iostream>

#include "sweater/Objects/Sphere.hpp"
#include "sweater/Objects/Plane.hpp"
#include "sweater/Objects/Torus.hpp"

#include "gtest/gtest.h"

TEST(CollisionTest, Sphere) {
  Eigen::Vector3f center(0,1,3);
  float radius = 1.7;
  Sphere ball(
      center,
      radius,
      1, // mass
      NULL);

  std::random_device rd;
  std::mt19937 randomGenerator(rd());

  Eigen::Vector3f closestPoint, closestNormal;

  Eigen::Vector3f randomPoint = 
    ball.generateRandomSurfacePoint(randomGenerator, ball.state);

  ASSERT_EQ((randomPoint - center).norm(), radius);

  Eigen::Vector3f outerPoint(-(radius+0.1), 1, 3);
  ASSERT_FALSE(ball.contains(outerPoint, ball.state, closestPoint, closestNormal));

  Eigen::Vector3f innerPoint(0.1, 1.3, 2.9);
  ASSERT_TRUE(ball.contains(innerPoint, ball.state, closestPoint, closestNormal));
}

TEST(CollisionTest, Plane) {
  Eigen::Vector3f planeNormal(0,0,1);
  float d = 0;
  Plane plane(
      planeNormal,
      0,
      NULL);

  Eigen::Vector3f closestPoint, closestNormal;

  Eigen::Vector3f outsidePlane(1,2,3);

  ASSERT_FALSE(plane.contains(outsidePlane, plane.state, closestPoint, closestNormal));

  Eigen::Vector3f insidePlane(1,2,-3);
  ASSERT_TRUE(plane.contains(insidePlane, plane.state, closestPoint, closestNormal));

  ASSERT_EQ(closestPoint, Eigen::Vector3f(1,2,0));
  ASSERT_EQ(closestNormal, planeNormal);
}

TEST(CollisionTest, Torus) {
  Eigen::Vector3f center(1,1,1);
  // point it up
  Eigen::Matrix3f rotationMatrix = 
    Eigen::AngleAxisf(M_PI/2., Eigen::Vector3f::UnitX()).toRotationMatrix();
  float majorRadius = 2;
  float minorRadius = 1;
  float mass = 1;

  Torus torus(
      center,
      rotationMatrix,
      majorRadius, 
      minorRadius,
      mass,
      NULL
      );

  std::random_device rd;
  std::mt19937 randomGenerator(rd());

  Eigen::Vector3f closestPoint, closestNormal;

  Eigen::Vector3f randomPoint = torus.generateRandomSurfacePoint(
      randomGenerator, 
      torus.state
      );

  float distanceFromCenter = (randomPoint - center).norm();

  ASSERT_TRUE((randomPoint - center).norm() <= majorRadius + minorRadius);

  ASSERT_TRUE((randomPoint - center).norm() >= majorRadius - minorRadius);

  ASSERT_TRUE(torus.contains(randomPoint, torus.state, closestPoint, closestNormal));

  //Eigen::Vector3f


  //Eigen::Vector3f outsidePlane(1,2,3);

  //ASSERT_FALSE(plane.contains(outsidePlane, plane.state, closestPoint, closestNormal));

  //Eigen::Vector3f insidePlane(1,2,-3);
  //ASSERT_TRUE(plane.contains(insidePlane, plane.state, closestPoint, closestNormal));

  //ASSERT_EQ(closestPoint, Eigen::Vector3f(1,2,0));
  //ASSERT_EQ(closestNormal, planeNormal);
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
