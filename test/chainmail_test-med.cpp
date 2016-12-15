#include <iostream>

#include "sweater/RayTracing/Renderer.hpp"
#include "sweater/RayTracing/Color.hpp"
#include "sweater/RayTracing/Camera.hpp"
#include "sweater/RayTracing/Ray.hpp"
#include "sweater/RayTracing/Hit.hpp"
#include "sweater/Lights/PointLight.hpp"
#include "sweater/Objects/Torus.hpp"
#include "sweater/Objects/Plane.hpp"
#include "sweater/Objects/Chainmail.hpp"
#include "sweater/Physics/TimeStepper.hpp"

int main() {
  std::string file = "./chainmail_test-med";
  int width = 720;
  int height = 720;
  double fps = 24;

  PerspectiveCamera camera(
      Eigen::Vector3f(0,0,0), // center
      Eigen::Vector3f(0,0,1), // direction: into the Z plane
      Eigen::Vector3f(0,1,0), // up is in Y
      M_PI/2.               // 90 degree viewing angle
      );

  PointLight light(
      Eigen::Vector3f(0,3,0), // into the scene and in front of object
      Color(1,1,1), // white light
      //0.1, // the radius
      0.0000001f // falloff
      );

  std::vector<Light *> lights = {&light};

  Material diffuseGrey(Color(0.3,0.3,0.4)); // just grey

  Eigen::Vector3f topLeftCorner(-2,2,4);
  float majorRadius = 0.4;
  float minorRadius = majorRadius * 0.15;
  float mass = 0.01;
  int rows = 10;
  int columns = 5;

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

  //ObjectVector objects({&ball});

  //std::vector<ObjectGroup *> objectGroups = {&chainmail, &objects};
  std::vector<ObjectGroup *> objectGroups = {&chainmail};

  Renderer renderer(
      file,
      width,
      height,
      fps,
      objectGroups,
      lights,
      &camera);

  //int length = 10 * 24;
  int length = 7 * 24;
  //int length = 2;
  float timeStep = 0.0015;

  for (int i = 0; i < length; i++) {
    renderer.renderFrame();
    std::cout << "frame = " << i << std::endl;
    for (int j = 0; j < 100; j++) {
      std::cout << "timestep = " << j << std::endl;
      RK4::takeStep(objectGroups, timeStep);
    }

  }
}
