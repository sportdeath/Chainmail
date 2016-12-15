#include <iostream>

#include "sweater/Objects/Sphere.hpp"
#include "sweater/Objects/ObjectGroup.hpp"
#include "sweater/Physics/TimeStepper.hpp"

#include "gtest/gtest.h"

TEST(PhysicsTest, BallsDrop) {
  Sphere ball(
      Eigen::Vector3f(0, 0, 0),
      1, // radius
      1, // mass
      NULL);

  ObjectGroup objects({&ball});

  float timeStep = 0.01;

  ASSERT_EQ(ball.state.momentum, Eigen::Vector3f(0,0,0));

  RK4::takeStep(objects, timeStep);

  // after 0.01 time momentum = 0 + force * 0.01
  // force is mass times gravity
  ASSERT_EQ(ball.state.momentum, timeStep * Object::GRAVITY);

  RK4::takeStep(objects, timeStep);

  ASSERT_EQ(ball.state.momentum, 2 * timeStep * Object::GRAVITY);

  RK4::takeStep(objects, timeStep);

  ASSERT_EQ(ball.state.momentum, 3 * timeStep * Object::GRAVITY);
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
