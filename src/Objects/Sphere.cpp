#include <iostream>
#include <limits>

#include "sweater/Objects/Sphere.hpp"

bool Sphere::intersect(
    const Ray & ray,
    Eigen::Vector3f center,
    float radius,
    float tmin,
    float & tplus,
    float & tminus) {
  const Eigen::Vector3f & rayOrigin = ray.getOrigin(); //Ray origin in the world coordinate
  const Eigen::Vector3f & dir = ray.getDirection();

  Eigen::Vector3f origin = rayOrigin - center;      //Ray origin in the sphere coordinate

  float a = dir.squaredNorm();
  float b = 2. * (dir.dot(origin));
  float c = origin.squaredNorm() - (radius * radius);

  // no intersection
  if (b * b - 4. * a * c < 0) {
    return false;
  }

  float d = sqrt(b * b - 4. * a * c);

  tplus = (-b + d) / (2.0f*a);
  tminus = (-b - d) / (2.0f*a);

  // the two intersections are at the camera back
  if ((tplus < tmin) && (tminus < tmin)) {
    return false;
  }

  return true;
}

bool Sphere::intersect(const Ray & ray, float tmin, Hit & hit) const{

  float tplus, tminus;
  if (not intersect(ray, state.position, radius, tmin, tplus, tminus)) return false;

  float t = std::numeric_limits<float>::max();
  // the two intersections are at the camera front
  if (tminus > tmin) {
    t = tminus;
  }

  // one intersection at the front. one at the back 
  if ((tplus > tmin) && (tminus < tmin)) {
    t = tplus;
  }

  if (t < hit.getT()) {
    Eigen::Vector3f normal =
      ray.pointAtParameter(t) - state.position;
    normal = normal.normalized();
    hit.set(t, getMaterial(), normal);
    return true;
  }

  return false;
}

void Sphere::computeInternalForcesAndConstraints(const ObjectState & state) {
  force += getMass() * Object::GRAVITY;
}

Eigen::Vector3f Sphere::generateRandomSurfacePoint(
    std::mt19937 & randomGenerator,
    const ObjectState & state
    ) const {

  std::uniform_real_distribution<> randomAngle(-M_PI,M_PI);

  float u = randomAngle(randomGenerator);
  float v = randomAngle(randomGenerator);

  Eigen::Vector3f point;

  point[0] = radius * std::cos(v) * std::cos(u);
  point[1] = radius * std::cos(v) * std::sin(u);
  point[2] = radius * std::sin(v);

  return pointToGlobal(point, state);
}

bool Sphere::contains(
    const Eigen::Vector3f & point, 
    const ObjectState & state,
    Eigen::Vector3f & closestPoint,
    Eigen::Vector3f & closestNormal
    ) const {
  float distance = (point - state.position).norm();
  if (distance > radius) return false;

  closestNormal = (point - state.position)/distance;
  closestPoint = closestNormal * radius;
  closestPoint = pointToGlobal(closestPoint, state);
  return true;
}
