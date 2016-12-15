#include <iostream>
#include <random>

#include "sweater/Objects/Torus.hpp"
#include "sweater/Objects/Sphere.hpp"
#include "sweater/Splines/PolyVector3f.hpp"

bool Torus::intersect(const Ray & ray, float tmin, Hit &hit) const{
  // first compute distance to center

  float tplus, tminus;
  if (not Sphere::intersect(
        ray, 
        getCenter(),
        majorRadius + minorRadius, 
        tmin, 
        tplus,
        tminus
        )) {
    return false;
  }

  // frame ray
  Ray frameRay = rayToLocal(ray, state);
  PolyVector3f coordinates(frameRay.getOrigin(), frameRay.getDirection());

  // x^2 + y^2 + z^2 + r^2 + R^2
  Polynomial leftHandSide = 
    coordinates.dot(coordinates) +
    ((majorRadius * majorRadius) -
    (minorRadius * minorRadius));

  // Square it
  Polynomial equation = leftHandSide * leftHandSide;

  // Select only x and z
  PolyVector3f filteredCoordinates = coordinates * Eigen::Vector3f(1, 1, 0);
  // 4R^2(x^2 + y^2)
  Polynomial rightHandSide =
    (4 * majorRadius * majorRadius) * 
    filteredCoordinates.dot(filteredCoordinates);

  // make it into a polynomial we can solve for root
  equation -= rightHandSide;

  std::vector<float> roots = equation.findRealRoots();

  bool foundIntersection = false;
  for (float root : roots) {
    if (root < tmin) continue;

    if (root < hit.getT()) {
      Eigen::Vector3f normal = 
        ((4 * leftHandSide.evaluate(root)) * 
           coordinates.evaluate(root)) - 
        ((8 * majorRadius * majorRadius) * 
           filteredCoordinates.evaluate(root));
      normal.normalize();
      normal = normalToGlobal(normal, state);
      hit.set(root, getMaterial(), normal);
      foundIntersection = true;
    }
  }
  return foundIntersection;
}

Eigen::Matrix3f Torus::getInertiaBodyTensor() const {
  return inertiaBodyTensor;
}

void Torus::computeInternalForcesAndConstraints(const ObjectState & state) {
  //torque = Eigen::Vector3f(0,0,0);
  force += 
    getMass() * Object::GRAVITY 
    - Object::DRAG * getVelocity(state);
  torque +=
    - Object::ANGULAR_DRAG * getAngularVelocity(state);
}

Eigen::Vector3f Torus::generateRandomSurfacePoint(
    std::mt19937 & randomGenerator,
    const ObjectState & state
    ) const {
  // use parametric equation
  std::uniform_real_distribution<> randomAngle(-M_PI, M_PI);
  float u = randomAngle(randomGenerator); // random between -Pi and Pi
  float v = randomAngle(randomGenerator);

  Eigen::Vector3f point;
  point[0] = 
    (majorRadius + minorRadius * std::cos(v)) * std::cos(u);
  point[1] = 
    (majorRadius + minorRadius * std::cos(v)) * std::sin(u);
  point[2] =
    minorRadius * std::sin(v);

  return pointToGlobal(point, state);
}

bool Torus::contains(
    const Eigen::Vector3f & point, 
    const ObjectState & state,
    Eigen::Vector3f & closestPoint,
    Eigen::Vector3f & closestNormal
    ) const {
  // put point in frame of reference
  Eigen::Vector3f pointInReference = Object::pointToLocal(point, state);

  // project point onto plane of circle
  Eigen::Vector3f pointOnCirclePlane = pointInReference;
  pointOnCirclePlane[2] = 0; // in z axis

  // find closest point on circle
  Eigen::Vector3f closestCirclePoint =
    majorRadius * pointOnCirclePlane.normalized();

  float distanceToCircle = 
    (pointInReference - closestCirclePoint).norm();

  if (distanceToCircle > minorRadius) return false;

  closestNormal = (pointInReference - closestCirclePoint)
    /distanceToCircle;
  closestPoint = minorRadius * closestNormal;
  
  closestNormal = normalToGlobal(closestNormal, state);
  closestPoint = pointToGlobal(closestPoint, state);
  return true;
}
