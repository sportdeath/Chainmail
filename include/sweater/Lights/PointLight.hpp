#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "sweater/Lights/Light.hpp"

class PointLight : public Light {
  public:
    PointLight(
        const Eigen::Vector3f & _position, 
        const Color & _color, 
        float _falloff) :
        position(_position),
        color(_color),
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
    float falloff;
};

#endif
