#include <iostream>

#include "sweater/RayTracing/Renderer.hpp"
#include "sweater/RayTracing/Color.hpp"
#include "sweater/RayTracing/Camera.hpp"
#include "sweater/RayTracing/Ray.hpp"
#include "sweater/RayTracing/Hit.hpp"
#include "sweater/Lights/SphericalLight.hpp"
#include "sweater/Objects/Torus.hpp"
#include "sweater/Objects/Plane.hpp"
#include "sweater/Objects/Chainmail.hpp"
#include "sweater/Physics/TimeStepper.hpp"

int main() {
  std::string file = "./chainmail_test";
  int width = 720;
  int height = 720;
  double fps = 24;

  PerspectiveCamera camera(
      Eigen::Vector3f(0,0,0), // center
      Eigen::Vector3f(0,0,1), // direction: into the Z plane
      Eigen::Vector3f(0,1,0), // up is in Y
      M_PI/2.               // 90 degree viewing angle
      );


  SphericalLight light2(
      Eigen::Vector3f(0,3,0), // into the scene and in front of object
      Color(1,1,1), // white light
      0.1, // the radius
      0.02f // falloff
      );

  std::vector<Light *> lights = {&light2};

  Material diffuseGrey(Color(0.3,0.3,0.4)); // just grey

  Eigen::Vector3f topLeftCorner(-1,1,3);
  float majorRadius = 0.4;
  float minorRadius = majorRadius * 0.15;
  float mass = 0.01;
  int rows = 3;
  int columns = 3;

  Chainmail chainmail(
      topLeftCorner,
      majorRadius,
      minorRadius,
      mass,
      rows,
      columns,
      &diffuseGrey
    );

  Material diffuseRed(Color(0.9,0.8,0.8)); // just grey

  Plane bigPlane(
      Eigen::Vector3f(0,1,0), // below light
      1, // at (0,-1,0)
      &diffuseRed
      );

  Eigen::Vector3f outOfFramePosition(100, 100, -100);
  Eigen::Vector3f outOfFrameMomentum(0, 0, 0);
  Eigen::Matrix3f outOfFrameRotationMatrix = Eigen::Matrix3f::Identity();
  Eigen::Vector3f outOfFrameAngularMomentum(0, 0, 0);

  Eigen::Vector3f inFramePosition(-4, 0, 3);
  Eigen::Matrix3f inFrameRotationMatrix = Eigen::Matrix3f::Identity();
  Eigen::Vector3f inFrameMomentum(0.6, 0, 0);
  Eigen::Vector3f inFrameAngularMomentum(0, 0, 0);

  Sphere ball(
      outOfFramePosition,
      0.7,
      0.1,
      &diffuseRed
      );

  ObjectVector objects({&bigPlane, &ball});

  //for (auto it = chainmail.begin();
      //it < chainmail.end();
      //it++) {
    //objects.addObject(*it);
  //}
  //std::vector<ObjectGroup *> objectGroups = {&objects};
  std::vector<ObjectGroup *> objectGroups = {&chainmail, &objects};

  Renderer renderer(
      file,
      width,
      height,
      fps,
      objectGroups,
      lights,
      &camera);

  int length = 2*48;
  float timeStep = 0.0015;

  for (int i = 0; i < length; i++) {
    if (i < length/4) {
      ball.state.position = outOfFramePosition;
      ball.state.rotationMatrix = outOfFrameRotationMatrix;
      ball.state.momentum = outOfFrameMomentum;
      ball.state.angularMomentum = outOfFrameAngularMomentum;
    } else if (i == length/4) {
      // throw ball
      ball.state.position = inFramePosition;
      ball.state.rotationMatrix = inFrameRotationMatrix;
      ball.state.momentum = inFrameMomentum;
      ball.state.angularMomentum = inFrameAngularMomentum;
    } else if (i >= length/4) {
      //renderer.renderFrame();
    }
    renderer.renderFrame();
    std::cout << "frame = " << i << std::endl;
    for (int j = 0; j < 100; j++) {
      std::cout << "timestep = " << j << std::endl;
      RK4::takeStep(objectGroups, timeStep);
    }
  }
}
