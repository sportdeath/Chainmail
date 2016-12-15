#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <eigen3/Eigen/Dense>

class Light {
  public:
    virtual ~Light() { }

    // in:  p           is the point to be shaded
    // out: tolight     is direction from p to light source
    // out: intensity   is the illumination intensity (RGB) at point p
    // out: distToLight is absolute distance from P to light (infinity for directional light)
    virtual void getIllumination(
        const Eigen::Vector3f & point, 
        Eigen::Vector3f & directionToLight, 
        Color & lightIntensity, 
        float & distToLight) const = 0;
};

#endif // LIGHT_H
