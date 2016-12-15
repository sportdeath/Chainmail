#include <iostream>

#include "sweater/Objects/Object.hpp"

// Physics
const Eigen::Vector3f Object::GRAVITY(0,-9.8,0);
const float Object::DRAG = 0.2; // was 0.3
const float Object::ANGULAR_DRAG = 0.003;
const int Object::COLLISION_SIZE = 1000;
const float Object::COLLISION_CONSTANT = 1000.;
const float Object::COLLISION_DAMPING = 50.;
const float Object::COLLISION_FRICTION = 100.;

Eigen::Vector3f Object::pointToLocal(
    const Eigen::Vector3f & point, 
    const ObjectState & state) {
  return vectorToLocal(point - state.position, state);
}

Eigen::Vector3f Object::pointToGlobal(
    const Eigen::Vector3f & point, 
    const ObjectState & state) {
  return vectorToGlobal(point, state) + state.position;
}

Eigen::Vector3f Object::vectorToLocal(
    const Eigen::Vector3f & vector,
    const ObjectState & state) {
  return state.rotationMatrix.transpose() * vector;
}

Eigen::Vector3f Object::vectorToGlobal(
    const Eigen::Vector3f & vector,
    const ObjectState & state) {
  return state.rotationMatrix * vector;
}

Eigen::Vector3f Object::normalToGlobal(
    const Eigen::Vector3f & normal, 
    const ObjectState & state) {
  return (state.rotationMatrix * normal).normalized();
}

Ray Object::rayToLocal(const Ray & ray, const ObjectState & state) {
  return Ray(
      pointToLocal(ray.getOrigin(), state), 
      vectorToLocal(ray.getDirection(), state)
      );
}

Eigen::Matrix3f Object::getInertiaTensorInv(const ObjectState & state) const {
  return
    state.rotationMatrix *
    getInertiaBodyTensor().inverse() * 
    state.rotationMatrix.transpose();
}

Eigen::Vector3f Object::getVelocity(const ObjectState & state) const {
  return state.momentum/mass;
}
Eigen::Vector3f Object::getAngularVelocity(const ObjectState & state) const {
  return 
    getInertiaTensorInv(state) *
    state.angularMomentum;
}

Eigen::Matrix3f Object::star(const Eigen::Vector3f  & vector) {
  Eigen::Matrix3f m;
  m << 
    0, -vector[2], vector[1],
    vector[2], 0, -vector[0],
    -vector[1], vector[0], 0;
  return m;
}
Eigen::Matrix3f Object::getRotationMatrixDot(const ObjectState & state) const {
  return star(getAngularVelocity(state)) * (state.rotationMatrix);
}

/**
 * Evaluates the derivative of a state.
 */
void Object::differentiateState(
    const ObjectState & state, 
    ObjectDerivative & derivative) const {
  derivative.velocity = getVelocity(state);
  derivative.rotationMatrixDot = getRotationMatrixDot(state); 
  derivative.force = force;
  derivative.torque = torque;
}

void Object::clearForceAndTorque() {
  // Gravity
  force = Eigen::Vector3f(0,0,0);
  torque = Eigen::Vector3f(0,0,0);
}

void Object::evaluateDerivative(
    const ObjectState & state, 
    const ObjectDerivative & derivative,
    const float timeStep,
    ObjectState & outputState) const {

  outputState.position = 
    state.position + timeStep * derivative.velocity;

  // rotate by timestep
  outputState.rotationMatrix =
    state.rotationMatrix + timeStep * derivative.rotationMatrixDot;

  outputState.momentum = 
    state.momentum + timeStep * derivative.force;

  outputState.angularMomentum = 
    state.angularMomentum + timeStep * derivative.torque;
}

void Object::multiplyDerivative(
    ObjectDerivative & derivative, const float factor) {
  derivative.velocity = factor * derivative.velocity;
  derivative.rotationMatrixDot = factor * derivative.rotationMatrixDot;
  derivative.force = factor * derivative.force;
  derivative.torque = factor * derivative.torque;
}

void Object::addDerivative(
    ObjectDerivative & derivative1,
    const ObjectDerivative & derivative2) {
  derivative1.velocity = 
    derivative1.velocity + 
    derivative2.velocity;
  derivative1.rotationMatrixDot =
    derivative1.rotationMatrixDot + 
    derivative2.rotationMatrixDot;
  derivative1.force = 
    derivative1.force + derivative2.force;
  derivative1.torque = 
    derivative1.torque + derivative2.torque;
}

Eigen::Vector3f Object::getVelocityOfPoint(
    const ObjectState & state,
    const Eigen::Vector3f & point) const {
  // v + omega x (p - position)
  return getVelocity(state) + 
    getAngularVelocity(state).cross(point - state.position);
}
