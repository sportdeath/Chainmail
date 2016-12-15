#include <iostream>

#include "sweater/Splines/PolyVector3f.hpp"

PolyVector3f PolyVector3f::cross(const PolyVector3f & p) const {
  return PolyVector3f(Eigen::Matrix<Polynomial, 3, 1>::cross(p));
}

Eigen::Vector3f PolyVector3f::evaluate(float x) const {
  return Eigen::Vector3f(
      (*this)[0].evaluate(x),
      (*this)[1].evaluate(x),
      (*this)[2].evaluate(x)
      );
}

PolyVector3f PolyVector3f::operator-() const {
  PolyVector3f p;
  for (int i = 0; i < 3; i++) {
    p[i] = -(*this)[i];
  }
  return p;
}

PolyVector3f & PolyVector3f::operator+=(const Eigen::Vector3f & c) {
  for (int i = 0; i < 3; i++) {
    (*this)[i] += c[i];
  }
  return *this;
}

PolyVector3f & PolyVector3f::operator-=(const Eigen::Vector3f & c) {
  for (int i = 0; i < 3; i++) {
    (*this)[i] -= c[i];
  }
  return *this;
}

PolyVector3f & PolyVector3f::operator*=(const Eigen::Vector3f & c) {
  for (int i = 0; i < 3; i++) {
    (*this)[i] *= c[i];
  }
  return *this;
}

PolyVector3f & PolyVector3f::operator/=(const Eigen::Vector3f & c) {
  for (int i = 0; i < 3; i++) {
    (*this)[i] /= c[i];
  }
  return *this;
}

PolyVector3f operator+(PolyVector3f p, const Eigen::Vector3f & c) {
  p += c;
  return p;
}

PolyVector3f operator+(const Eigen::Vector3f & c, PolyVector3f p) {
  p += c;
  return p;
}

PolyVector3f operator-(PolyVector3f p, const Eigen::Vector3f & c) {
  p -= c;
  return p;
}

PolyVector3f operator-(const Eigen::Vector3f & c, PolyVector3f p) {
  p = -p;
  return p + c;
}

PolyVector3f operator*(PolyVector3f p, const Eigen::Vector3f & c) {
  p *= c;
  return p;
}

PolyVector3f operator*(const Eigen::Vector3f & c, PolyVector3f p) {
  p *= c;
  return p;
}

PolyVector3f operator/(PolyVector3f p, const Eigen::Vector3f & c) {
  p /= c;
  return p;
}
