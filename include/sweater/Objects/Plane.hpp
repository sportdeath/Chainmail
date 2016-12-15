#ifndef PLANE_HPP
#define PLANE_HPP

#include <random>

#include "sweater/Objects/Object.hpp"

class Plane : public Object {
  public:
    Plane(
        const Eigen::Vector3f & _normal,
        float _d,
        Material * _material) : 
        Object(_material),
        normal(_normal),
        d(_d) 
    { }

    Eigen::Vector3f getNormal() const {
      return normal;
    }
    float getD() const {return d;}

    virtual bool intersect(const Ray &r, float rmin, Hit &h) const override;

    virtual Eigen::Matrix3f getInertiaBodyTensor() const override {
      return Eigen::Matrix3f::Identity();
    }
    virtual void computeInternalForcesAndConstraints(const ObjectState & state) override {
      force = Eigen::Vector3f::Zero();
      torque = Eigen::Vector3f::Zero();
    };

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
    Eigen::Vector3f normal;
    float d;
};

#endif
