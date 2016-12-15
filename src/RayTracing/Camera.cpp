#include <eigen3/Eigen/Dense>

#include "sweater/RayTracing/Camera.hpp"

PerspectiveCamera::PerspectiveCamera(
    const Eigen::Vector3f & _center,
    const Eigen::Vector3f & _direction,
    const Eigen::Vector3f & _up,
    float _angleRadians) :
    center(_center),
    direction(_direction.normalized()),
    up(_up),
    angleRadians(_angleRadians)
{
  horizontal = (direction.cross(up)).normalized();
  float d = 1.0f / (float)std::tan(angleRadians / 2.0f);
  angledDirection = d * direction;
}

Ray PerspectiveCamera::generateRay(float x, float y) {
  Eigen::Vector3f newDir = angledDirection + x * horizontal + y * up;
  newDir.normalize();

  return Ray(center, newDir);
}

float PerspectiveCamera::getTMin() const {
  return 0.0f;
}
