#include <iostream>

#include "sweater/Objects/Plane.hpp"

bool Plane::intersect(const Ray &r, float tmin, Hit &h) const {
  float t = -(d + normal.dot(r.getOrigin()))
    /(normal.dot(r.getDirection()));

  if (t < tmin) return false;

  if (t < h.getT()) {
    h.set(t, getMaterial(), getNormal());
    return true;
  }

  return false;
}

Eigen::Vector3f Plane::generateRandomSurfacePoint(
    std::mt19937 & randomGenerator,
    const ObjectState & state) const {
  return -d * normal;
}

bool Plane::contains(
    const Eigen::Vector3f & point, 
    const ObjectState & state,
    Eigen::Vector3f & closestPoint,
    Eigen::Vector3f & closestNormal
    ) const {
  float distance = point.dot(normal) + d;
  if (distance > 0) return false;
  
  closestNormal = normal;
  closestPoint = point - normal * distance;
  return true;
}
