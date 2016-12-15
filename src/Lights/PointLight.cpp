#include <limits>

#include <eigen3/Eigen/Dense>

#include "sweater/RayTracing/Color.hpp"
#include "sweater/Lights/PointLight.hpp"

void PointLight::getIllumination(
    const Eigen::Vector3f & point, 
    Eigen::Vector3f & directionToLight,
    Color & lightIntensity,
    float & distanceToLight) const {

  directionToLight = position - point;
  distanceToLight = directionToLight.norm();
  directionToLight = directionToLight/distanceToLight;

  float fallOffFactor = distanceToLight * distanceToLight * falloff;

  if (fallOffFactor < 1) {
    lightIntensity = color;
  } else {
    lightIntensity = color / fallOffFactor;
  }
}

