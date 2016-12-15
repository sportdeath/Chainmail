#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <array>
#include <random>

#include <iostream>
#include <csignal>

#include "sweater/RayTracing/Ray.hpp"
#include "sweater/RayTracing/Hit.hpp"
#include "sweater/RayTracing/Material.hpp"

typedef struct ObjectState {
  Eigen::Vector3f position; // x(t)
  Eigen::Matrix3f rotationMatrix; // q(t)
  Eigen::Vector3f momentum; // P(t)
  Eigen::Vector3f angularMomentum; // L(t)
} ObjectState;

typedef struct ObjectDerivative {
  Eigen::Vector3f velocity; // dx(t)/dt = v(t) = P(t)/M
  Eigen::Matrix3f rotationMatrixDot; // dR(t)/dt = omega(t) * R(t) 
                                   // omega(t)= I(t)^-1 L(t)
  Eigen::Vector3f force; // dP(t)/dt = F(t)
  Eigen::Vector3f torque; // dL(t)/dt = Torque(t)
} ObjectDerivative;

class Object {
  public:
    Object(
        Material * _material = NULL,
        float _mass = 1,
        Eigen::Vector3f _position = Eigen::Vector3f::Zero(),
        Eigen::Matrix3f _rotationMatrix = Eigen::Matrix3f::Identity(),
        Eigen::Vector3f _momentum = Eigen::Vector3f::Zero(),
        Eigen::Vector3f _angularMomentum = Eigen::Vector3f::Zero()
        ) :
        material(_material),
        mass(_mass)
        {

      state = {
        _position,
        _rotationMatrix,
        _momentum,
        _angularMomentum
      };
      trialState = state;
    }

    Material * getMaterial() const {
      return material;
    }

    // Ray tracing
    virtual bool intersect(const Ray &ray, float tmin, Hit &hit) const = 0;

    static Eigen::Vector3f pointToLocal(
        const Eigen::Vector3f & point,
        const ObjectState & state);

    static Eigen::Vector3f pointToGlobal(
        const Eigen::Vector3f & point,
        const ObjectState & state);

    static Eigen::Vector3f vectorToLocal(
        const Eigen::Vector3f & vector,
        const ObjectState & state);

    static Eigen::Vector3f vectorToGlobal(
        const Eigen::Vector3f & vector,
        const ObjectState & state);

    static Eigen::Vector3f normalToGlobal(
        const Eigen::Vector3f & normal, 
        const ObjectState & state);

    static Ray rayToLocal(const Ray & ray, const ObjectState & state);


    // Physics
    virtual Eigen::Matrix3f getInertiaBodyTensor() const = 0;

    Eigen::Matrix3f getInertiaTensorInv(const ObjectState & state) const;

    Eigen::Vector3f getVelocity(const ObjectState & state) const;

    Eigen::Vector3f getAngularVelocity(const ObjectState & state) const;

    static Eigen::Matrix3f star(const Eigen::Vector3f  & vector);

    Eigen::Matrix3f getRotationMatrixDot(const ObjectState & state) const;

    /**
     * Evaluates the derivative of a state.
     */
    void differentiateState(
        const ObjectState & state, 
        ObjectDerivative & derivative) const;

    void clearForceAndTorque();

    virtual void computeInternalForcesAndConstraints(const ObjectState & state) = 0;
    void computeInternalForcesAndConstraints() {
      return computeInternalForcesAndConstraints(trialState);
    }

    void evaluateDerivative(
        const ObjectState & state, 
        const ObjectDerivative & derivative,
        const float timeStep,
        ObjectState & outputState) const;

    static void multiplyDerivative(
        ObjectDerivative & derivative, const float factor);

    static void addDerivative(
        ObjectDerivative & derivative1,
        const ObjectDerivative & derivative2);

    Eigen::Vector3f getVelocityOfPoint(
        const ObjectState & state,
        const Eigen::Vector3f & point) const;

    float getMass() {return mass;}

    // for collisions
    virtual Eigen::Vector3f generateRandomSurfacePoint(
        std::mt19937 & randomGenerator,
        const ObjectState & state
        ) const = 0;

    virtual bool contains(
        const Eigen::Vector3f & point,
        const ObjectState & state,
        Eigen::Vector3f & closestPoint,
        Eigen::Vector3f & closestNormal
        ) const = 0;

    void collide(Object * other) {
      return collide(trialState, other, other -> trialState);
    }

    void collide(
        const ObjectState & state, // my state
        Object * other,
        const ObjectState & otherState
        ) {
      std::random_device rd;
      std::mt19937 randomGenerator(rd());

      Eigen::Vector3f restoringForce(0,0,0);
      Eigen::Vector3f restoringTorque(0,0,0);
      
      Eigen::Vector3f closestPoint;
      Eigen::Vector3f closestNormal;
      for (int i = 0; i < COLLISION_SIZE; i++) {
        Eigen::Vector3f point = 
          generateRandomSurfacePoint(randomGenerator, state);

        if (other -> 
            contains(
              point, 
              otherState, 
              closestPoint, 
              closestNormal)) {

          Eigen::Vector3f relativeVelocity =
            getVelocityOfPoint(state, point) -
            other -> getVelocityOfPoint(otherState, closestPoint);

          Eigen::Vector3f pointForce = 
            // k * ||x - c||
            (
            COLLISION_CONSTANT * 
            (point - closestPoint).norm()
            -
            //// k2 * ||v1 - v2||
            COLLISION_DAMPING *
            relativeVelocity.dot(closestNormal)
            ) * 
            closestNormal;
          
          // friction
          // Damping in tangent plane
          Eigen::Vector3f tangentPlane =
            -relativeVelocity.cross(closestNormal);
          Eigen::Vector3f repulsivePlane =
            tangentPlane.cross(closestNormal);
          float repulsiveMagnitude = repulsivePlane.norm();
          if (repulsiveMagnitude != 0) {
            repulsivePlane = repulsivePlane/repulsiveMagnitude;
            pointForce -= 
              COLLISION_FRICTION * 
              repulsivePlane *
              relativeVelocity.dot(repulsivePlane);
          }

          restoringForce += pointForce;
          restoringTorque += (point - state.position).cross(pointForce);
        }
      }

      restoringForce /= float(COLLISION_SIZE);
      restoringTorque /= float(COLLISION_SIZE);
      force += restoringForce;
      torque += restoringTorque;
    }
    
  public:
    ObjectState state;
    ObjectState trialState;
    std::array<ObjectDerivative, 4> derivatives;
    Eigen::Vector3f force;
    Eigen::Vector3f torque;
    static const Eigen::Vector3f GRAVITY;
    static const float DRAG;
    static const float ANGULAR_DRAG;
    static const int COLLISION_SIZE;
    static const float COLLISION_CONSTANT;
    static const float COLLISION_DAMPING;
    static const float COLLISION_FRICTION;

  private:
    Material * material;
    float mass;
};

#endif
