#include <limits>

#include <eigen3/Eigen/Dense>

#include "sweater/RayTracing/Color.hpp"
#include "sweater/Lights/SphericalLight.hpp"

void SphericalLight::getIllumination(
    const Eigen::Vector3f & point, 
    Eigen::Vector3f & directionToLight,
    Color & lightIntensity,
    float & distanceToLight) const {

  Eigen::Vector3f positionOnLight = 
    position + radius * Eigen::Vector3f::Random().normalized();

  directionToLight = positionOnLight - point;
  distanceToLight = directionToLight.norm();
  directionToLight = directionToLight/distanceToLight;

  float fallOffFactor = distanceToLight * distanceToLight * falloff;

  if (fallOffFactor < 1) {
    lightIntensity = color;
  } else {
    lightIntensity = color / fallOffFactor;
  }
}

