#ifndef HIT_HPP
#define HIT_HPP

#include <eigen3/Eigen/Dense>

class Material;
class Hit {
  public:
    // Constructors
    Hit() :
        material(NULL),
        t(std::numeric_limits<float>::max()),
        normal(0,0,0)
    {}

    Hit(
        float t, 
        Material *_material, 
        const Eigen::Vector3f & _normal) :
        t(t),
        material(_material),
        normal(_normal)
    {
    }

    float getT() const {
        return t;
    }

    Material * getMaterial() const {
        return material;
    }

    const Eigen::Vector3f getNormal() const {
        return normal;
    }

    void set(
        float t, 
        Material * material, 
        const Eigen::Vector3f & normal) {
        this->t = t;
        this->material = material;
        this->normal = normal;
    }

    float t;
    Material * material;
    Eigen::Vector3f normal;
};

inline std::ostream &
operator<<(std::ostream &os, const Hit &h)
{
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

#endif
