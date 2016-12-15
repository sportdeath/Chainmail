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
  std::string file = "./chainmail_test-large";
  int width = 360;
  int height = 360;
  double fps = 24;

  PerspectiveCamera camera(
      Eigen::Vector3f(0,0,0), // center
      Eigen::Vector3f(0,0,1), // direction: into the Z plane
      Eigen::Vector3f(0,1,0), // up is in Y
      M_PI/2.               // 90 degree viewing angle
      );

  //SphericalLight light1(
      //Eigen::Vector3f(0,3,0), // into the scene and in front of object
      //Color(1,1,1), // white light
      //0.25,
      //0.01f // falloff
      //);

  SphericalLight light2(
      Eigen::Vector3f(0,3,0), // into the scene and in front of object
      Color(.8,.8,1), // white light
      0.1, // the radius
      0.01f // falloff
      );

  std::vector<Light *> lights = {&light2};

  Material diffuseGrey(Color(0.3,0.3,0.4)); // just grey

  Eigen::Vector3f topLeftCorner(-1,1,6);
  float majorRadius = 0.1;
  float minorRadius = majorRadius * 0.1;
  float mass = 0.01 * (majorRadius * majorRadius)/(0.4 * 0.4);
  int rows = 5;
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

  Material diffuseRed(Color(1,1,1)); // just grey

  Plane bigPlane(
      Eigen::Vector3f(0,1,0), // below light
      1, // at (0,-1,0)
      &diffuseRed
      );

  ObjectVector objects({&bigPlane});

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

  int length = 48;
  float timeStep = 0.005;

  for (int i = 0; i < length; i++) {
    renderer.renderFrame();
    std::cout << "frame = " << i << std::endl;
    for (int j = 0; j < 30; j++) {
      std::cout << "timestep = " << j << std::endl;
      RK4::takeStep(objectGroups, timeStep);
    }
  }
}
