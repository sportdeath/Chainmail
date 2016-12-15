#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <eigen3/Eigen/Dense>

#include "sweater/RayTracing/Ray.hpp"

class Camera {
  public:
    virtual ~Camera() {}

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(float x, float y) = 0;
    virtual float getTMin() const = 0;
};

class PerspectiveCamera : public Camera {
  public:
    PerspectiveCamera(const Eigen::Vector3f & _center,
        const Eigen::Vector3f & _direction,
        const Eigen::Vector3f & _up,
        float _angleRadians);

    virtual Ray generateRay(float x, float y) override;

    virtual float getTMin() const override;

private:
    Eigen::Vector3f center;
    Eigen::Vector3f direction;
    Eigen::Vector3f up;
    float angleRadians;
    Eigen::Vector3f angledDirection;
    Eigen::Vector3f horizontal;
};

#endif //CAMERA_HPP
