#include <iostream>

#include "sweater/RayTracing/Renderer.hpp"
#include "sweater/RayTracing/Color.hpp"
#include "sweater/RayTracing/Camera.hpp"
#include "sweater/RayTracing/Ray.hpp"
#include "sweater/RayTracing/Hit.hpp"
#include "sweater/Lights/SphericalLight.hpp"
#include "sweater/Objects/Torus.hpp"
#include "sweater/Objects/Plane.hpp"
#include "sweater/Physics/TimeStepper.hpp"

int main() {
  std::string file = "./torus_test";
  int width = 360;
  int height = 360;
  double fps = 24;

  PerspectiveCamera camera(
      Eigen::Vector3f(0,0,0), // center
      Eigen::Vector3f(0,0,1), // direction: into the Z plane
      Eigen::Vector3f(0,1,0), // up is in Y
      M_PI/2.               // 90 degree viewing angle
      );

  SphericalLight light1(
      Eigen::Vector3f(0,3,2), // into the scene and in front of object
      Color(1,1,1), // white light
      0.25,
      0.01f // falloff
      );

  SphericalLight light2(
      Eigen::Vector3f(-4,3,3), // into the scene and in front of object
      Color(.8,.8,1), // white light
      0.1, // the radius
      0.02f // falloff
      );

  std::vector<Light *> lights = {&light1, &light2};

  Material diffuseGrey(Color(0.3,0.3,0.4)); // just grey

  Torus torus1(
      Eigen::Vector3f(-1, 1, 4),
      Eigen::Matrix3f::Identity(),
      1.5,
      0.3,
      0.1,
      &diffuseGrey);

  Torus torus2(
      Eigen::Vector3f(0.5, 1, 4),
      Eigen::AngleAxisf(M_PI/2., Eigen::Vector3f::UnitX()).toRotationMatrix(),
      1.5,
      0.3,
      0.1,
      &diffuseGrey);

  Material diffuseGreen(Color(0,0.6,0)); // just grey

  Plane bigPlane(
      Eigen::Vector3f(0,1,0), // below light
      1, // at (0,-1,0)
      &diffuseGreen
      );

  ObjectVector objects({
      &torus1,
      &torus2,
      &bigPlane,
      });

  std::vector<ObjectGroup *> objectGroups = {&objects};

  Renderer renderer(
      file,
      width,
      height,
      fps,
      objectGroups,
      lights,
      &camera);

  int length = 2*12;
  float timeStep = 0.005;

  for (int i = 0; i < length; i++) {
    renderer.renderFrame();
    for (int j = 0; j < 10; j++) {
      std::cout << "physics step: " << j << std::endl;
      RK4::takeStep(objectGroups, timeStep);
    }
  }
}
