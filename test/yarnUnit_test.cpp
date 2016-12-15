#include <iostream>

#include "sweater/RayTracing/Renderer.hpp"
#include "sweater/RayTracing/Color.hpp"
#include "sweater/RayTracing/Camera.hpp"
#include "sweater/Lights/SphericalLight.hpp"
#include "sweater/Objects/YarnUnit.hpp"
#include "sweater/Objects/Plane.hpp"

int main() {
  std::string file = "./yarnUnit_test";
  int width = 50;
  int height = 50;
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

  std::vector<Eigen::Vector3f> yarnPoints = {
    Eigen::Vector3f(-1, 1, 4),
    Eigen::Vector3f(0, 2, 4),
    Eigen::Vector3f(1, 1, 4),
    Eigen::Vector3f(2, 0, 4),
  };

  YarnUnit yarnUnit(
      yarnPoints.begin(),
      0.4,
      &diffuseGrey);

  Material diffuseGreen(Color(0,0.6,0)); // just grey

  Plane bigPlane(
      Eigen::Vector3f(0,1,0), // below light
      -1, // at (0,-1,0)
      &diffuseGreen
      );

  Material diffusePurple(Color(0.44,0.28,0.68)); // purple

  Plane backPlane(
      Eigen::Vector3f(0,0,-1), // facing back towards the camera
      -7, // at (0,-1,0)
      &diffusePurple
      );

  ObjectGroup objects({
      &yarnUnit,
      &bigPlane,
      &backPlane
      });

  Renderer renderer(
      file,
      width,
      height,
      fps,
      &objects,
      lights,
      &camera);

  renderer.renderFrame();
}
