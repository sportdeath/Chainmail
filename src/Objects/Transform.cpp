#include <iostream>

#include "sweater/Objects/Transform.hpp"

bool Transform::intersect(const Ray & ray, float tmin, Hit & hit) const
{
  // transform ray
  Eigen::Vector3f newOrigin = 
    inverseTransform * ray.getOrigin(); // a point

  Eigen::Vector3f newDirection = 
    inverseTransform.linear() * ray.getDirection(); // a vector

  Ray newRay = Ray(newOrigin, newDirection);

  bool isIntersection = object -> intersect(newRay, tmin, hit);

  if (isIntersection) {
    // transform h
    Eigen::Vector3f normal = 
      (inverseTransform.linear().transpose() * hit.getNormal()).head<3>();
    normal.normalize();
    hit.set(hit.getT(), hit.getMaterial(), normal);

    return true;
  }

  return false;
}

void Transform::transform(const Eigen::Affine3f & m) {
  inverseTransform = inverseTransform * m.inverse();
}
