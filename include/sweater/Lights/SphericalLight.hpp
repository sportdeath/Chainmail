#ifndef SPHERICAL_LIGHT_HPP
#define SPHERICAL_LIGHT_HPP

#include "sweater/Lights/Light.hpp"

class SphericalLight : public Light {
  public:
    SphericalLight(
        const Eigen::Vector3f & _position, 
        const Color & _color, 
        float _radius,
        float _falloff) :
        position(_position),
        color(_color),
        radius(_radius),
        falloff(_falloff)
    { }

    virtual void getIllumination(
        const Eigen::Vector3f & point, 
        Eigen::Vector3f & directionToLight, 
        Color & lightIntensity, 
        float & distToLight) const override;

  private:
    Eigen::Vector3f position;
    Color color;
    float radius;
    float falloff;
};

#endif
