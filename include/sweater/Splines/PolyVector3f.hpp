#ifndef POLYVECTOR3F_HPP
#define POLYVECTOR3F_HPP

#include <eigen3/Eigen/Dense>

#include "sweater/Splines/polynomial.hpp"

class PolyVector3f : public Eigen::Matrix<Polynomial, 3, 1> {
  public:
    PolyVector3f(
        const Polynomial & p1,
        const Polynomial & p2,
        const Polynomial & p3
        ) : Eigen::Matrix<Polynomial, 3, 1>(p1, p2, p3) {}

    PolyVector3f() : Eigen::Matrix<Polynomial, 3, 1>() {}

    PolyVector3f(const Eigen::Matrix<Polynomial, 3, 1> & base)
      : Eigen::Matrix<Polynomial, 3, 1>(base) {}

    PolyVector3f(const Eigen::Vector3f & vector)
      : Eigen::Matrix<Polynomial, 3, 1>(
          Polynomial( vector[0] ),
          Polynomial( vector[1] ),
          Polynomial( vector[2] )
          ) {}

    PolyVector3f(const Eigen::Vector3f & constant, const Eigen::Vector3f & slope)
      : Eigen::Matrix<Polynomial, 3, 1>(
          Polynomial( {constant[0], slope[0]} ),
          Polynomial( {constant[1], slope[1]} ),
          Polynomial( {constant[2], slope[2]} )
          ) {}

    Eigen::Vector3f evaluate(float x) const; 

    PolyVector3f cross(const PolyVector3f & p) const;

    PolyVector3f operator-() const;
    PolyVector3f &operator+=(const Eigen::Vector3f & c);
    PolyVector3f &operator-=(const Eigen::Vector3f & c);
    PolyVector3f &operator*=(const Eigen::Vector3f & c);
    PolyVector3f &operator/=(const Eigen::Vector3f & c);

    friend PolyVector3f operator+(PolyVector3f p, const Eigen::Vector3f & c);
    friend PolyVector3f operator+(const Eigen::Vector3f & c, PolyVector3f p);
    friend PolyVector3f operator-(PolyVector3f p, const Eigen::Vector3f & c);
    friend PolyVector3f operator-(const Eigen::Vector3f & c, PolyVector3f p);
    friend PolyVector3f operator*(PolyVector3f p, const Eigen::Vector3f & c);
    friend PolyVector3f operator*(const Eigen::Vector3f & c, PolyVector3f p);
    friend PolyVector3f operator/(PolyVector3f p, const Eigen::Vector3f & c);

    inline friend std::ostream & operator<<(
        std::ostream &os, const PolyVector3f & p) {
      os << 
        p[0] << "\n" <<
        p[1] << "\n" <<
        p[2];
      return os;
    }
};

#endif
