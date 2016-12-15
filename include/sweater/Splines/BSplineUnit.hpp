#ifndef BSPLINE_UNIT_HPP
#define BSPLINE_UNIT_HPP

#include <array>

#include <eigen3/Eigen/Dense>

#include "sweater/Splines/polynomial.hpp"
#include "sweater/Splines/polyvector3f.hpp"

class BSplineUnit {
  public:
    BSplineUnit(std::vector<Eigen::Vector3f>::iterator startingPoint);

    const PolyVector3f & getPolynomials() const {return polynomials;}
    const PolyVector3f & getPolynomialsP() const {return polynomialsP;}
    const PolyVector3f & getPolynomialsPP() const {return polynomialsPP;}

  private:
    static const std::array<Polynomial,4> BSplineBasis;
    static const std::array<Polynomial,4> BSplineBasisP;
    static const std::array<Polynomial,4> BSplineBasisPP;

    PolyVector3f polynomials;
    PolyVector3f polynomialsP;
    PolyVector3f polynomialsPP;
};

//#include <initializer_list>
//#include <vector>

//class BSpline {
  //private:
    //std::vector<Eigen::Vector3f> points;

    //// 1 -3t  3t^2 -1t^3
    //// 0  3t -6t^2  3t^3
    //// 0   0  3t^2 -3t^3
    //// 0   0   0     t^3
    //static const Eigen::Matrix4f BSplineBasis;
    //static const Eigen::Matrix4f BSplineBasisP;
    //static const Eigen::Matrix4f BSplineBasisPP;
  //public:
    //BSpline(std::vector<Eigen::Vector3f> & points);

    ////Eigen::Vector3f evaluate(
        ////iterator startingPoint
        ////Eigen::Matrix4f geometryMatrix,
        ////Eigen::Vector4f powerMatrix
        ////);

    ////Eigen::Vector3f evaluateP(
        ////Point * startingPoint
        ////Eigen::Matrix4f geometryMatrix,
        ////Eigen::Vector4f powerMatrix
        ////);

    ////Eigen::Vector3f evaluatePP(
        ////Point * startingPoint
        ////Eigen::Matrix4f geometryMatrix,
        ////Eigen::Vector4f powerMatrix
        ////);

    //const Eigen::Matrix4f & getGeometryMatrix();

    /**
     * Returns a vector of polynomials
     * starting from startingIndex
     * startingIndex < numPoints - 3
     */
    //PolyVector3f & getPolyVector(iterator startingPoint);

    //// second and first derivatives
    //PolyVector3f & getPolyVectorP(int startingIndex);
    //PolyVector3f & getPolyVectorPP(int startingIndex);
//};

#endif
