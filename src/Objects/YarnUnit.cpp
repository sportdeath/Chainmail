#include <iostream>

#include "sweater/RayTracing/Ray.hpp"
#include "sweater/RayTracing/Hit.hpp"
#include "sweater/Splines/polynomial.hpp"
#include "sweater/Splines/polyvector3f.hpp"
#include "sweater/Splines/bsplineunit.hpp"
#include "sweater/Objects/YarnUnit.hpp"

bool YarnUnit::intersect(
    const Ray & ray, 
    float tmin, 
    Hit & hit) const {

  const PolyVector3f direction(ray.getDirection());


  // t'(u) x (d x (t(u) - o))
  const PolyVector3f positionInPlane = 
    splineUnit.getPolynomialsP().cross(
        direction.cross(
          splineUnit.getPolynomials() - ray.getOrigin()
          )
        );

  //std::cout << "Direction\n" << direction << std::endl;
  //std::cout << "Origin\n" << ray.getOrigin() << std::endl;
  //std::cout << "Polynomial\n" << splineUnit.getPolynomials() << std::endl;
  //std::cout << "PolynomialP\n" << splineUnit.getPolynomialsP() << std::endl;
  //std::cout << "Position in plane\n" << positionInPlane << std::endl;

  // (t' x t'')
  const PolyVector3f crossPandPP = 
    splineUnit.getPolynomialsP().cross(
        splineUnit.getPolynomialsPP()
        );

  std::cout << "CrossP and PP\n" << crossPandPP << std::endl;

  // e2 component
  Polynomial e2Component = 
    splineUnit.getPolynomialsPP().dot(positionInPlane);
  e2Component *= e2Component;
  e2Component *= 
    splineUnit.getPolynomialsP().dot(splineUnit.getPolynomialsP());

  std::cout << "e2 Component\n" << e2Component << std::endl;

  // e3 component
  Polynomial e3Component = 
    crossPandPP.dot(positionInPlane);
  e3Component *= e3Component;

  std::cout << "e3 Component\n" << e3Component << std::endl;

  // r * (t' dot d)
  Polynomial rightHandSide = 
    radius * 
    (splineUnit.getPolynomialsP().dot(direction));
  std::cout << "rightHandSide:\n" << rightHandSide << std::endl;
  // (r * (t' dot d))^2
  rightHandSide *= rightHandSide;
  std::cout << "rightHandSide:\n" << rightHandSide << std::endl;
  // (r * (t' dot d) * |t' x t''|)^2
  rightHandSide *= (crossPandPP.dot(crossPandPP));
  std::cout << "rightHandSide:\n" << rightHandSide << std::endl;


  const Polynomial toSolve = e2Component + e3Component - rightHandSide;

  std::cout << "toSolve\n" << toSolve << std::endl;


  std::vector<float> roots = 
    toSolve.findRealRoots();

  bool foundIntersection = false;
  for (float root : roots) {
    if ((root < 0) or (root > 1)) continue;

    std::cout << root << std::endl;

    Eigen::Vector3f position =
      splineUnit.getPolynomials().evaluate(root);
    Eigen::Vector3f velocity =
      splineUnit.getPolynomialsP().evaluate(root);
    float s = 
      (velocity.dot(position - ray.getOrigin()))/
      (velocity.dot(ray.getDirection()));

    Eigen::Vector3f normal = 
      ray.pointAtParameter(s) - position;
    std::cout << "Nomral:\n" << normal << std::endl;
    std::cout << "Norm: " << normal.norm() << std::endl;
    normal.normalize();

    if (s < tmin) continue;

    if (s < hit.getT()) {
      hit.set(s, this -> material, normal);
      foundIntersection = true;
    }
  }

  return foundIntersection;
}
