#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <vector>
#include <initializer_list>

#include <eigen3/Eigen/Dense>

class Polynomial {
  private:
    static const float ROOT_THRESHOLD;
    std::vector<float> coefficients;

    Polynomial(std::vector<float> & _coefficients);

  public:
    // Construct from coefficients list
    Polynomial();
    Polynomial(float constant);
    Polynomial(std::initializer_list<float> _coefficients);

    // observer
    int getSize() const;
    float getCoefficient(int index) const;

    float evaluate(float input) const;
    std::vector<float> findRealRoots() const;

    Polynomial operator-() const;
    Polynomial &operator+=(const Polynomial & p);
    Polynomial &operator-=(const Polynomial & p);
    Polynomial &operator*=(const Polynomial & p);

    Polynomial &operator+=(float c);
    Polynomial &operator-=(float c);
    Polynomial &operator*=(float c);
    Polynomial &operator/=(float c);

    // multiply, add, subtract polynomials
    friend bool operator==(const Polynomial & p1, const Polynomial & p2);
    friend Polynomial operator+(Polynomial p1, const Polynomial &p2);
    friend Polynomial operator-(Polynomial p1, const Polynomial &p2);
    friend Polynomial operator*(Polynomial p1, const Polynomial &p2);

    // multiply, add, subtract, divide constants
    friend Polynomial operator+(Polynomial p, float c);
    friend Polynomial operator+(float c, Polynomial p);
    friend Polynomial operator-(Polynomial p, float c);
    friend Polynomial operator-(float c, Polynomial p);
    friend Polynomial operator*(Polynomial p, float c);
    friend Polynomial operator*(float c, Polynomial p);
    friend Polynomial operator/(Polynomial p, float c);


    inline friend std::ostream & operator<<(
        std::ostream &os, const Polynomial & p) {
      os << p.getCoefficient(0);
      for (int i = 1; i < p.getSize(); i++) {
        os << " + " << p.getCoefficient(i) << "x^" << i;
      }
      return os;
    }
};

#endif
