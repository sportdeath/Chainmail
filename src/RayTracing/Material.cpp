#include <iostream>

#include "sweater/RayTracing/Material.hpp"

Eigen::Vector3f Material::shade(
    const Ray &ray,
    const Hit &hit,
    const Eigen::Vector3f &dirToLight,
    const Eigen::Vector3f &lightIntensity)
{

  Eigen::Vector3f normal = hit.getNormal();

  // Compute angle with light
  float diffuseShading = dirToLight.dot(normal);
  if (diffuseShading <= 0) {
    return Eigen::Vector3f(0,0,0);
  }

  // Scale diffuse color by angle and light intensity
  Color diffuseIllumination = diffuseShading * lightIntensity;
  diffuseIllumination = 
    diffuseIllumination.cwiseProduct(getColor());

  return diffuseIllumination;
}
