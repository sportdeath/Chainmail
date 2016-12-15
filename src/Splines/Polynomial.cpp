#include <initializer_list>
#include <iostream>

#include <eigen3/Eigen/Eigenvalues>

#include "sweater/Splines/polynomial.hpp"

const float Polynomial::ROOT_THRESHOLD = 0.00000001;

Polynomial::Polynomial() : Polynomial(0) {}

Polynomial::Polynomial(float constant) {
  coefficients = {constant};
}

Polynomial::Polynomial(std::initializer_list<float> _coefficients) {
  coefficients.assign(
      _coefficients.begin(),
      _coefficients.end()
      );

  assert(coefficients.size() >= 1);
}

Polynomial::Polynomial(std::vector<float> & _coefficients) {
  coefficients = _coefficients;

  assert(coefficients.size() >= 1);
}

float Polynomial::getCoefficient(int index) const {
  assert(index < coefficients.size());
  return coefficients[index];
}

int Polynomial::getSize() const {
  return coefficients.size();
}

float Polynomial::evaluate(float input) const {

  std::vector<float>::const_reverse_iterator it = coefficients.rbegin();

  float output = *it;
  it++;

  for (; it != coefficients.rend(); it++) {

    output = *it + output * input;
  }

  return output;
}

std::vector<float> Polynomial::findRealRoots() const {
  int maxIndex = coefficients.size() - 1;
  Eigen::MatrixXf companion(maxIndex, maxIndex);

  // fill top row with zero
  companion.topLeftCorner(1, maxIndex - 1) 
    = Eigen::MatrixXf::Zero(1, maxIndex - 1);

  // fill bottom with identity
  companion.bottomLeftCorner(maxIndex - 1, maxIndex - 1)
    = Eigen::MatrixXf::Identity(maxIndex - 1, maxIndex - 1);

  // fill with coefficients
  for (int i = 0; i < maxIndex; i++) {
    companion(i, maxIndex - 1) = -coefficients[i]/coefficients[maxIndex];
  }

  // compute eigen values -> roots
  Eigen::VectorXcf eigenValues = companion.eigenvalues();

  std::vector<float> output;
  
  // choose only real roots
  for (int i = 0; i < eigenValues.rows(); i++) {
    if (std::abs(eigenValues[i].imag()) < ROOT_THRESHOLD) {
      output.push_back(eigenValues[i].real());
    }
  }
  
  return output;
}

bool operator==(const Polynomial & p1, const Polynomial & p2) {
  if (p1.getSize() != p2.getSize()) return false;
  for (int i = 0; i < p1.getSize(); i++) {
    if (p1.getCoefficient(i) != p2.getCoefficient(i)) {
      return false;
    }
  }
  return true;
}

// multiply, add, subtract polynomials
Polynomial Polynomial::operator-() const {
  Polynomial p = *this;
  for (int i = 0; i < p.getSize(); i++) {
    p.coefficients[i] = -p.coefficients[i];
  }
  return p;
}

Polynomial & Polynomial::operator+=(const Polynomial &p) {
  coefficients.resize(std::max(getSize(),p.getSize()));

  for (int i = 0; i < p.getSize(); i++) {
    coefficients[i] += p.coefficients[i];
  }
  return *this;
}

Polynomial & Polynomial::operator-=(const Polynomial &p) {
  coefficients.resize(std::max(getSize(),p.getSize()));

  for (int i = 0; i < p.getSize(); i++) {
    coefficients[i] -= p.coefficients[i];
  }
  return *this;
}

Polynomial & Polynomial::operator*=(const Polynomial &p) {
  std::vector<float> newCoefficients(getSize() + p.getSize() - 1);
  std::fill(newCoefficients.begin(), newCoefficients.end(), 0);

  for (int i = 0; i < getSize(); i++) {
    for (int j = 0; j < p.getSize(); j++) {
      newCoefficients[i + j] += coefficients[i] * p.coefficients[j];
    }
  }

  coefficients = newCoefficients;

  return *this;
}

// multiply, add, subtract, divide constants

Polynomial & Polynomial::operator+=(float c) {
  coefficients[0] += c;
  return *this;
}

Polynomial & Polynomial::operator-=(float c) {
  coefficients[0] -= c;
  return *this;
}

Polynomial & Polynomial::operator*=(float c) {
  for (int i = 0; i < getSize(); i++) {
    coefficients[i] *= c;
  }
  return *this;
}

Polynomial & Polynomial::operator/=(float c) {
  for (int i = 0; i < getSize(); i++) {
    coefficients[i] /= c;
  }
  return *this;
}

// Binary operators
Polynomial operator+(Polynomial p1, const Polynomial &p2) {
  p1 += p2;
  return p1;
}

Polynomial operator-(Polynomial p1, const Polynomial &p2) {
  p1 -= p2;
  return p1;
}

Polynomial operator*(Polynomial p1, const Polynomial &p2) {
  p1 *= p2;
  return p1;
}

Polynomial operator+(float c, Polynomial p) {
  p += c;
  return p;
}

Polynomial operator+(Polynomial p, float c) {
  p += c;
  return p;
}

Polynomial operator-(float c, Polynomial p) {
  p = -p;
  return p + c;
}

Polynomial operator-(Polynomial p, float c) {
  p -= c;
  return p;
}

Polynomial operator*(float c, Polynomial p) {
  p *= c;
  return p;
}

Polynomial operator*(Polynomial p, float c) {
  p *= c;
  return p;
}

Polynomial operator/(Polynomial p, float c) {
  p /= c;
  return p;
}
