#ifndef RAY_HPP
#define RAY_HPP

#include <eigen3/Eigen/Dense>

class Ray {
  public:
    Ray(
        const Eigen::Vector3f & _origin, 
        const Eigen::Vector3f & _direction) :
        origin(_origin),
        direction(_direction)
    {}

    static Ray randomRay(
        const Eigen::Vector3f & origin, 
        const Eigen::Vector3f & normal,
        float & dotProduct) {
      Eigen::Vector3f randomHemisphere;
      while (true) {
        randomHemisphere = Eigen::Vector3f::Random().normalized();
        dotProduct = randomHemisphere.dot(normal);
        if (dotProduct > 0) {
          return Ray(origin, randomHemisphere);
        }
      }
    }

    const Eigen::Vector3f & getOrigin() const {
        return origin;
    }

    const Eigen::Vector3f & getDirection() const {
        return direction;
    }

    Eigen::Vector3f pointAtParameter(float t) const {
        return origin + (direction * t);
    }

  private:
    Eigen::Vector3f origin;
    Eigen::Vector3f direction;
};

inline std::ostream &
operator<<(std::ostream &os, const Ray &r)
{
    os << "Ray <" << r.getOrigin() << ", " << r.getDirection() << ">";
    return os;
}

#endif // RAY_HPP
