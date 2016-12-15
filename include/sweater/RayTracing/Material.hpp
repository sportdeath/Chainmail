#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <eigen3/Eigen/Dense>

#include "sweater/RayTracing/Ray.hpp"
#include "sweater/RayTracing/Hit.hpp"
#include "sweater/RayTracing/Color.hpp"

class Material {
  public:

    Material(const Color & _color) : color(_color) {}

    const Color & getColor() const {
      return color;
    }

    Color shade(const Ray & ray,
        const Hit & hit,
        const Eigen::Vector3f & dirToLight,
        const Color & lightIntensity);

protected:
    Color color;
};

#endif // MATERIAL_HPP
