#ifndef TORUS_HPP
#define TORUS_HPP

#include <random>

#include "sweater/Objects/Object.hpp"
#include "sweater/Objects/Sphere.hpp"

class Torus : public Object {
  public:
    Torus(
        const Eigen::Vector3f & _center,
        const Eigen::Matrix3f & _rotationMatrix,
        float _majorRadius,
        float _minorRadius,
        float _mass,
        Material * _material) :
      Object(
          _material, 
          _mass,
          _center,
          _rotationMatrix
          ),
      majorRadius(_majorRadius),
      minorRadius(_minorRadius)
  {
    float xyInertia =
      ((5/8. * _majorRadius * _majorRadius) + 
       (1/2. * _minorRadius * _minorRadius)) 
      * _mass;
    float zInertia = 
      ((3/4. * _majorRadius * _majorRadius) + 
       (_minorRadius * _minorRadius)) 
      * _mass;

    inertiaBodyTensor << 
      xyInertia, 0, 0,
      0, xyInertia, 0,
      0, 0, zInertia;
  }

    virtual bool intersect(const Ray &r, float tmin, Hit &h) const override;

    const Eigen::Vector3f & getCenter() const {return state.position;}

    virtual Eigen::Matrix3f getInertiaBodyTensor() const override;
    virtual void computeInternalForcesAndConstraints(const ObjectState & state) override;

    virtual Eigen::Vector3f generateRandomSurfacePoint(
        std::mt19937 & randomGenerator,
        const ObjectState & state) const override;

    bool contains(
        const Eigen::Vector3f & point,
        const ObjectState & state,
        Eigen::Vector3f & closestPoint,
        Eigen::Vector3f & closestNormal
        ) const override;

  private:
    float majorRadius;
    float minorRadius;
    Eigen::Matrix3f inertiaBodyTensor;
};

#endif
