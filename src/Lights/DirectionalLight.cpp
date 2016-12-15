#include <limits>

#include <eigen3/Eigen/Dense>

#include "sweater/RayTracing/Color.hpp"
#include "sweater/Lights/DirectionalLight.hpp"

void DirectionalLight::getIllumination(
    const Eigen::Vector3f & point, 
    Eigen::Vector3f & directionToLight,
    Color & lightIntensity,
    float & distanceToLight) const {

    directionToLight = -direction;
    lightIntensity  = color;
    distanceToLight = std::numeric_limits<float>::max();
}

void PointLight::getIllumination(
    const Eigen::Vector3f & point, 
    Eigen::Vector3f & directionToLight,
    Color & lightIntensity,
    float & distanceToLight) const {

  directionTolight = position - point;
  distanceToLight = directionTolight.abs();
  directionTolight.normalize();

  lightIntensity = color / (distanceToLight * distanceToLight * falloff);
}

