#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <random>

#include "sweater/Objects/Object.hpp"

class Sphere : public Object {
  public:
    Sphere(
        const Eigen::Vector3f & _center,
        float _radius,
        float _mass,
        Material * _material) :
      Object(_material,_mass,_center),
      radius(_radius) {
      float inertia = 2/5. * _mass * _radius * _radius;
      inertiaBodyTensor << 
        inertia, 0, 0,
        0, inertia, 0,
        0, 0, inertia;
    }

    static bool intersect(
        const Ray & ray, 
        Eigen::Vector3f center, 
        float radius, 
        float tmin,
        float & tplus,
        float & tminus);

    virtual bool intersect(const Ray &r, float tmin, Hit &h) const override;

    virtual Eigen::Matrix3f getInertiaBodyTensor() const override {
      return inertiaBodyTensor;
    }

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
    float radius;
    Eigen::Matrix3f inertiaBodyTensor;
};

#endif
