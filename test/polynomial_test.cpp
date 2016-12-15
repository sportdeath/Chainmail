#include <sweater/Splines/polynomial.hpp>
#include <sweater/Splines/polyvector3f.hpp>

#include "gtest/gtest.h"

TEST(PolynomialTest, BlankInitialize) {
  Polynomial p;

  ASSERT_EQ(p.getCoefficient(0), 0);
  ASSERT_EQ(p.getSize(), 1);
}

TEST(PolynomialTest, Initialize) {
  Polynomial p ( {3, 6, 1} );

  ASSERT_EQ(p.getCoefficient(0), 3);
  ASSERT_EQ(p.getCoefficient(1), 6);
  ASSERT_EQ(p.getCoefficient(2), 1);
  ASSERT_EQ(p.getSize(), 3);
}

TEST(PolynomialOps, SameSizeAdd) {
  Polynomial p1 ( {6, 3, 1} );
  Polynomial p2 ( {2, 6, 9} );

  Polynomial p3 = p1 + p2;

  // P1 and P2 unchanged
  ASSERT_EQ(p1.getCoefficient(0), 6);
  ASSERT_EQ(p1.getCoefficient(1), 3);
  ASSERT_EQ(p1.getCoefficient(2), 1);
  ASSERT_EQ(p1.getSize(), 3);
  ASSERT_EQ(p2.getCoefficient(0), 2);
  ASSERT_EQ(p2.getCoefficient(1), 6);
  ASSERT_EQ(p2.getCoefficient(2), 9);
  ASSERT_EQ(p2.getSize(), 3);
  
  // P3 correct
  ASSERT_EQ(p3.getCoefficient(0), 8);
  ASSERT_EQ(p3.getCoefficient(1), 9);
  ASSERT_EQ(p3.getCoefficient(2), 10);
  ASSERT_EQ(p3.getSize(), 3);
}

TEST(PolynomialOps, DifferentSizeAdd) {
  Polynomial p1 ( {4, 7, 5, 7} );
  Polynomial p2 ( {6, 3, 1, 6, 9, 7} );

  Polynomial p3 = p1 + p2;
  ASSERT_EQ(p3.getCoefficient(0), 10);
  ASSERT_EQ(p3.getCoefficient(1), 10);
  ASSERT_EQ(p3.getCoefficient(2), 6);
  ASSERT_EQ(p3.getCoefficient(3), 13);
  ASSERT_EQ(p3.getCoefficient(4), 9);
  ASSERT_EQ(p3.getCoefficient(5), 7);
  ASSERT_EQ(p3.getSize(), 6);
}

TEST(PolynomialOps, SameSizeSub) {
  Polynomial p1 ( {6, 3, 1} );
  Polynomial p2 ( {2, 6, 9} );

  Polynomial p3 = p1 - p2;

  // P1 and P2 unchanged
  ASSERT_EQ(p1.getCoefficient(0), 6);
  ASSERT_EQ(p1.getCoefficient(1), 3);
  ASSERT_EQ(p1.getCoefficient(2), 1);
  ASSERT_EQ(p1.getSize(), 3);
  ASSERT_EQ(p2.getCoefficient(0), 2);
  ASSERT_EQ(p2.getCoefficient(1), 6);
  ASSERT_EQ(p2.getCoefficient(2), 9);
  ASSERT_EQ(p2.getSize(), 3);
  
  // P3 correct
  ASSERT_EQ(p3.getCoefficient(0), 4);
  ASSERT_EQ(p3.getCoefficient(1), -3);
  ASSERT_EQ(p3.getCoefficient(2), -8);
  ASSERT_EQ(p3.getSize(), 3);
}

TEST(PolynomialOps, DifferentSizeSub) {
  Polynomial p1 ( {4, 7, 5, 7} );
  Polynomial p2 ( {6, 3, 1, 6, 9, 7} );

  Polynomial p3 = p2 - p1;
  ASSERT_EQ(p3.getCoefficient(0), 2);
  ASSERT_EQ(p3.getCoefficient(1), -4);
  ASSERT_EQ(p3.getCoefficient(2), -4);
  ASSERT_EQ(p3.getCoefficient(3), -1);
  ASSERT_EQ(p3.getCoefficient(4), 9);
  ASSERT_EQ(p3.getCoefficient(5), 7);
  ASSERT_EQ(p3.getSize(), 6);
}

TEST(PolynomialOps, SameSizeMul) {
  Polynomial p1 ( {6, 3, 1} );
  Polynomial p2 ( {2, 6, 9} );

  Polynomial p3 = p1 * p2;
  ASSERT_EQ(p3.getCoefficient(0), 12);
  ASSERT_EQ(p3.getCoefficient(1), 42);
  ASSERT_EQ(p3.getCoefficient(2), 74);
  ASSERT_EQ(p3.getCoefficient(3), 33);
  ASSERT_EQ(p3.getCoefficient(4), 9);
  ASSERT_EQ(p3.getSize(), 5);
}

TEST(PolynomialOps, DifferentSizeMul) {
  Polynomial p1 ( {4, 7, 5, 7} );
  Polynomial p2 ( {6, 3, 1, 6, 9, 7} );

  Polynomial p3 = p1 * p2;
  ASSERT_EQ(p3.getCoefficient(0), 24);
  ASSERT_EQ(p3.getCoefficient(1), 54);
  ASSERT_EQ(p3.getCoefficient(2), 55);
  ASSERT_EQ(p3.getCoefficient(3), 88);
  ASSERT_EQ(p3.getCoefficient(4), 104);
  ASSERT_EQ(p3.getCoefficient(5), 128);
  ASSERT_EQ(p3.getCoefficient(6), 136);
  ASSERT_EQ(p3.getCoefficient(7), 98);
  ASSERT_EQ(p3.getCoefficient(8), 49);
  ASSERT_EQ(p3.getSize(), 9);
}

TEST(PolynomialTest, Evaluate) {
  Polynomial p1 ( {4.3, 7.54, 5.2, 7.0} );

  float out = p1.evaluate(3.14);
  ASSERT_NEAR(out, 295.96, 0.01);
}

TEST(PolynomialTest, FindRoots) {
  Polynomial p ( {4.3, -7.54, 5.2, -7.0, -8.2, 0, 0, 0, 0, 0, 0, 0, 10.} );

  std::vector<float> roots = p.findRealRoots();

  bool root1 = false, root2 = false;
  for (float root : roots) {
    root1 |= std::abs(root - (1.03495)) < 0.0001;
    root2 |= std::abs(root - (0.536261)) < 0.0001;
  }

  ASSERT_TRUE(root1);
  ASSERT_TRUE(root2);

  ASSERT_EQ(roots.size(), 2);
}

TEST(PolynomialOps, ConstAdd) {
  Polynomial p1 ( {4, 7, 5, 7} );

  Polynomial p2 = p1 + 38.1618;
  ASSERT_NEAR(p2.getCoefficient(0), 42.1618, 0.00001);
  ASSERT_EQ(p2.getCoefficient(1), 7);
  ASSERT_EQ(p2.getCoefficient(2), 5);
  ASSERT_EQ(p2.getCoefficient(3), 7);
  ASSERT_EQ(p2.getSize(), 4);
}
TEST(PolynomialOps, ConstSub) {
  Polynomial p1 ( {4, 7, 5, 7} );

  Polynomial p2 = p1 - 38.1618;
  ASSERT_NEAR(p2.getCoefficient(0), -34.1618, 0.00001);
  ASSERT_EQ(p2.getCoefficient(1), 7);
  ASSERT_EQ(p2.getCoefficient(2), 5);
  ASSERT_EQ(p2.getCoefficient(3), 7);
  ASSERT_EQ(p2.getSize(), 4);
}
TEST(PolynomialOps, ConstSubInverse) {
  Polynomial p1 ( {4, 7, 5, 7} );

  Polynomial p2 = 38.1618 - p1;
  ASSERT_NEAR(p2.getCoefficient(0), 34.1618, 0.00001);
  ASSERT_EQ(p2.getCoefficient(1), -7);
  ASSERT_EQ(p2.getCoefficient(2), -5);
  ASSERT_EQ(p2.getCoefficient(3), -7);
  ASSERT_EQ(p2.getSize(), 4);
}
TEST(PolynomialOps, ConstMul) {
  Polynomial p1 ( {4, 7, 5, 7} );

  Polynomial p2 = 1.5 * p1;
  ASSERT_EQ(p2.getCoefficient(0), 6);
  ASSERT_EQ(p2.getCoefficient(1), 10.5);
  ASSERT_EQ(p2.getCoefficient(2), 7.5);
  ASSERT_EQ(p2.getCoefficient(3), 10.5);
  ASSERT_EQ(p2.getSize(), 4);
}
TEST(PolynomialOps, ConstDiv) {
  Polynomial p1 ( {4, 7, 5, 7} );

  Polynomial p2 = p1/2.;
  ASSERT_EQ(p2.getCoefficient(0), 2);
  ASSERT_EQ(p2.getCoefficient(1), 3.5);
  ASSERT_EQ(p2.getCoefficient(2), 2.5);
  ASSERT_EQ(p2.getCoefficient(3), 3.5);
  ASSERT_EQ(p2.getSize(), 4);
}

TEST(PolyVector, BasicInit) {
  PolyVector3f p(Eigen::Vector3f(1, 2, 3));
  ASSERT_EQ(p[0].getCoefficient(0), 1);
  ASSERT_EQ(p[1].getCoefficient(0), 2);
  ASSERT_EQ(p[2].getCoefficient(0), 3);
}

TEST(PolyVector, AdditionWithVector) {
  PolyVector3f p(Eigen::Vector3f(1, 2, 3));
  Eigen::Vector3f q(4, 7, 5);
  PolyVector3f r = p + q;

  ASSERT_EQ(r[0].getCoefficient(0), 5);
  ASSERT_EQ(r[1].getCoefficient(0), 9);
  ASSERT_EQ(r[2].getCoefficient(0), 8);
}

TEST(PolyVector, SubtractionWithVector) {
  PolyVector3f p(Eigen::Vector3f(1, 2, 3));
  Eigen::Vector3f q(4, 7, 5);
  PolyVector3f r = p - q;

  ASSERT_EQ(r[0].getCoefficient(0), -3);
  ASSERT_EQ(r[1].getCoefficient(0), -5);
  ASSERT_EQ(r[2].getCoefficient(0), -2);
}

TEST(PolyVector, CrossProduct) {
  PolyVector3f p(Eigen::Vector3f(1, 2, 3));
  PolyVector3f q(Eigen::Vector3f(3, 6, 1));
  PolyVector3f r = p.cross(q);

  ASSERT_EQ(r[0].getCoefficient(0), -16);
  ASSERT_EQ(r[1].getCoefficient(0), 8);
  ASSERT_EQ(r[2].getCoefficient(0), 0);
}

TEST(PolyVector, DotItself) {
  PolyVector3f p(
      Polynomial({1, 4, 5, 2}),
      Polynomial({2, 3, 2}),
      Polynomial({1, 4, 5, 2, 10})
      );

  Polynomial r = p.dot(p);

  ASSERT_EQ(r.getCoefficient(0), 6);
  ASSERT_EQ(r.getCoefficient(1), 28);
  ASSERT_EQ(r.getCoefficient(2), 69);
  ASSERT_EQ(r.getCoefficient(3), 100);
  ASSERT_EQ(r.getCoefficient(4), 106);
  ASSERT_EQ(r.getCoefficient(5), 120);
  ASSERT_EQ(r.getCoefficient(6), 108);
  ASSERT_EQ(r.getCoefficient(7), 40);
  ASSERT_EQ(r.getCoefficient(8), 100);
}

TEST(PolyVector, Evaluate) {
  PolyVector3f p(
      Polynomial({1, 4, 5, 2}),
      Polynomial({2, 3, 2}),
      Polynomial({1, 4, 5, 2, 10})
      );

  Eigen::Vector3f out = p.evaluate(3.14);

  ASSERT_NEAR(out[0], 124.776, 0.01);
  ASSERT_NEAR(out[1], 31.1392, 0.01);
  ASSERT_NEAR(out[2], 1096.89, 0.01);
}

TEST(PolyVector, AdditionWithVector2) {
  PolyVector3f p(
      Polynomial({1, 4, 5, 2}),
      Polynomial({2, 3, 2}),
      Polynomial({1, 4, 5, 2, 10})
      );

  PolyVector3f q = p;

  Eigen::Vector3f x(4, 7, 5);
  p+=x;

  ASSERT_EQ(p[0].getCoefficient(0), 5);
  ASSERT_EQ(p[1].getCoefficient(0), 9);
  ASSERT_EQ(p[2].getCoefficient(0), 6);

  ASSERT_EQ(p[0].getCoefficient(1), 4);
  ASSERT_EQ(p[1].getCoefficient(1), 3);
  ASSERT_EQ(p[2].getCoefficient(1), 4);

  ASSERT_EQ(p, q+x);
  ASSERT_EQ(p, x+q);
  ASSERT_EQ(q+x, x+q);
}

TEST(PolyVector, SubtractionWithVector2) {
  PolyVector3f p(
      Polynomial({1, 4, 5, 2}),
      Polynomial({2, 3, 2}),
      Polynomial({1, 4, 5, 2, 10})
      );

  PolyVector3f q = p;

  Eigen::Vector3f x(4, 7, 5);
  p-=x;

  ASSERT_EQ(p[0].getCoefficient(0), -3);
  ASSERT_EQ(p[1].getCoefficient(0), -5);
  ASSERT_EQ(p[2].getCoefficient(0), -4);

  ASSERT_EQ(p[0].getCoefficient(1), 4);
  ASSERT_EQ(p[1].getCoefficient(1), 3);
  ASSERT_EQ(p[2].getCoefficient(1), 4);

  ASSERT_EQ(p, q-x);
  ASSERT_EQ(-p, x-q);
  ASSERT_EQ(q-x, -(x-q));
}

TEST(PolyVector, MultiplicationWithVector) {
  PolyVector3f p(
      Polynomial({1, 4, 5, 2}),
      Polynomial({2, 3, 2}),
      Polynomial({1, 4, 5, 2, 10})
      );
  PolyVector3f q = p;

  Eigen::Vector3f x(4, 7, 5);
  p*=x;

  ASSERT_EQ(p[0].getCoefficient(0), 4);
  ASSERT_EQ(p[1].getCoefficient(0), 14);
  ASSERT_EQ(p[2].getCoefficient(0), 5);

  ASSERT_EQ(p[0].getCoefficient(1), 16);
  ASSERT_EQ(p[1].getCoefficient(1), 21);
  ASSERT_EQ(p[2].getCoefficient(1), 20);

  ASSERT_EQ(p[0].getCoefficient(2), 20);
  ASSERT_EQ(p[1].getCoefficient(2), 14);
  ASSERT_EQ(p[2].getCoefficient(2), 25);

  ASSERT_EQ(p, q*x);
  ASSERT_EQ(p, x*q);
  ASSERT_EQ(q*x, x*q);
}

TEST(PolyVector, DivisionWithVector) {
  PolyVector3f p(
      Polynomial({1, 4, 5, 2}),
      Polynomial({2, 3, 2}),
      Polynomial({1, 4, 5, 2, 10})
      );

  PolyVector3f q = p;

  Eigen::Vector3f x(4, 7, 5);
  p/=x;

  ASSERT_NEAR(p[0].getCoefficient(0), 1/4., 0.01);
  ASSERT_NEAR(p[1].getCoefficient(0), 2/7., 0.01);
  ASSERT_NEAR(p[2].getCoefficient(0), 1/5., 0.01);

  ASSERT_NEAR(p[0].getCoefficient(1), 4/4., 0.01);
  ASSERT_NEAR(p[1].getCoefficient(1), 3/7., 0.01);
  ASSERT_NEAR(p[2].getCoefficient(1), 4/5., 0.01);

  ASSERT_EQ(p, q/x);
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
