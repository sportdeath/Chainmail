//BSpline::getGeometryMatrix(iterator startingPoint) {

//}

//getPolyVector(
    //Poly1 = 
    //Point1[0] * Poly[0]
    //+ Point2[0] * Poly[1]
    //+ Point3[0] * Poly[2]
    //+ Point4[0] * Poly[3]

    //Poly2 = 
    //Point1[1] * Poly[0]
    //+ Point2[1] * Poly[1]
    //+ Point3[1] * Poly[2]
    //+ Point4[1] * Poly[3]

#include <iostream>

#include "sweater/Splines/BSplineUnit.hpp"
#include "sweater/Splines/Polynomial.hpp"

BSplineUnit::BSplineUnit(
    std::vector<Eigen::Vector3f>::iterator startingPoint
    ) {
  // XYZ
  for (int dimension = 0; dimension < 3; dimension++) {
    // Control point
    for (int point = 0; point < 4; point++) {
      float coordinate = (*(startingPoint + point))[dimension];

      // update polynomials
      polynomials[dimension] += coordinate * BSplineBasis[point];
      polynomialsP[dimension] += coordinate * BSplineBasisP[point];
      polynomialsPP[dimension] += coordinate * BSplineBasisPP[point];
    }
  }
}

const std::array<Polynomial,4> BSplineUnit::BSplineBasis = {
  Polynomial({1/6., -3/6., 3/6., -1/6.}),
  Polynomial({4/6.,  0/6.,-6/6.,  3/6.}),
  Polynomial({1/6.,  3/6., 3/6., -3/6.}),
  Polynomial({   0,     0,    0,  1/6.})
};

const std::array<Polynomial,4> BSplineUnit::BSplineBasisP = {
  Polynomial({-3/6.,  6/6., -3/6.}),
  Polynomial({ 0/6.,-12/6.,  9/6.}),
  Polynomial({ 3/6.,  6/6., -9/6.}),
  Polynomial({    0,     0,  3/6.})
};

const std::array<Polynomial,4> BSplineUnit::BSplineBasisPP = {
  Polynomial({  6/6., -6/6.}),
  Polynomial({-12/6.,  18/6.}),
  Polynomial({  6/6., -18/6.}),
  Polynomial({     0,  6/6.})
};
