#include <iostream>

#include "sweater/RayTracing/Renderer.hpp"
#include "sweater/RayTracing/Color.hpp"
#include "sweater/RayTracing/Camera.hpp"
#include "sweater/RayTracing/Ray.hpp"
#include "sweater/RayTracing/Hit.hpp"
#include "sweater/Lights/SphericalLight.hpp"
#include "sweater/Objects/Torus.hpp"
#include "sweater/Objects/Plane.hpp"

#include "gtest/gtest.h"

TEST(TorusTest, BasicTest) {
  Torus torus(
      Eigen::Vector3f(1, 0, 1),
      Eigen::Matrix3f::Identity(),
      1,
      0.5,
      1,
      NULL);

  Ray ray(
      Eigen::Vector3f(0,0,0),// origin
      Eigen::Vector3f(0,0,1)// direction
      );

  Hit hit;

  bool intersect = torus.intersect(ray,0,hit);
  ASSERT_TRUE(intersect);

  Eigen::Vector3f pointOfIntersection = ray.pointAtParameter(hit.getT());
  ASSERT_NEAR(pointOfIntersection[0], 0,   0.00001);
  ASSERT_NEAR(pointOfIntersection[1], 0,   0.00001);
  ASSERT_NEAR(pointOfIntersection[2], 0.5, 0.00001);
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
