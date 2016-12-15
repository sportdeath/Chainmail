#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <eigen3/Eigen/Dense>

#include "sweater/Objects/Object.hpp"

class Transform : public Object {
  public:
    Transform(const Eigen::Affine3f & m,
        Object * _object) : Object(NULL), object(_object) {
      inverseTransform = m.inverse();
    }

    virtual bool intersect(const Ray &r, float tmin, Hit &h) const override;

    void transform(const Eigen::Affine3f & m);
    
  private:
    Object * object;
    Eigen::Affine3f inverseTransform;
};

#endif
