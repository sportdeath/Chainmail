#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

class DirectionalLight : public Light {
  public:
    DirectionalLight(
        const Eigen::Vector3f & _direction, 
        const Color & _color) :
        direction(_direction.normalized()),
        color(c)
    { }

    virtual void getIllumination(
        const Eigen::Vector3f & point, 
        Eigen::Vector3f & directionToLight, 
        Color & lightIntensity, 
        float & distToLight) const override;

  private:
    Eigen::Vector3f direction;
    Color color;
};

#endif
