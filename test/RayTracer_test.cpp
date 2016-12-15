#include <iostream>

#include "sweater/RayTracing/Renderer.hpp"
#include "sweater/RayTracing/Color.hpp"
#include "sweater/RayTracing/Camera.hpp"
#include "sweater/Lights/SphericalLight.hpp"
#include "sweater/Objects/Sphere.hpp"
#include "sweater/Objects/Plane.hpp"
#include "sweater/Objects/ObjectGroup.hpp"
#include "sweater/Physics/TimeStepper.hpp"

int main() {
  std::string file = "./raytracer_test";
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
      0.05f // falloff
      );

  SphericalLight light2(
      Eigen::Vector3f(-4,3,3), // into the scene and in front of object
      Color(.8,.8,1), // white light
      0.1, // the radius
      0.02f // falloff
      );

  std::vector<Light *> lights = {&light1, &light2};

  Material diffuseGrey(Color(0.8,0.6,0.6)); // just grey

  Sphere centerSphere(
      Eigen::Vector3f(-1,1,4), // below light
      0.3, // radius of 1
      0.1f, // mass
      &diffuseGrey
      );

  Sphere sphere2(
      Eigen::Vector3f(1.3,1,4), // below light
      0.3, // radius of 1
      0.1f, // mass
      &diffuseGrey
      );

  Material diffuseGreen(Color(0,0.5,0)); // green

  Plane bigPlane(
      Eigen::Vector3f(0,1,0), // below light
      1, // at (0,-1,0)
      &diffuseGreen
      );

  Material diffusePurple(Color(0.44,0.28,0.68)); // purple

  Plane backPlane(
      Eigen::Vector3f(0,0,-1), // facing back towards the camera
      -7, // at (0,-1,0)
      &diffusePurple
      );

  ObjectVector objects({
      &centerSphere,
      &sphere2,
      &bigPlane
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

  centerSphere.state.momentum = Eigen::Vector3f(0.1, 0, 0);

  int length = 2*48;
  float timestep = 0.0005;
  for (int i = 0; i < length; i++) {
    renderer.renderFrame();
    for (int i = 0; i < 100; i++) {
      RK4::takeStep(objectGroups, timestep);
      if (std::isnan(centerSphere.state.position[0])) {
        std::cout << "uh oh" << std::endl;
        return 0;
      }
    }
  }

}
