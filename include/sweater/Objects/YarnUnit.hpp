#ifndef YARN_UNIT_HPP
#define YARN_UNIT_HPP

#include "sweater/RayTracing/Ray.hpp"
#include "sweater/RayTracing/Hit.hpp"
#include "sweater/Objects/Object.hpp"
#include "sweater/Splines/BSplineUnit.hpp"

class YarnUnit : public Object {
  public:
    YarnUnit(
        std::vector<Eigen::Vector3f>::iterator startingPoint,
        float _radius,
        Material * _material
        ) : splineUnit(startingPoint),
            radius(_radius),
            Object(_material) {}

    virtual bool intersect(
        const Ray & ray, 
        float tmin, 
        Hit & hit) const override;

  private:
    BSplineUnit splineUnit;
    float radius;
};

#endif
