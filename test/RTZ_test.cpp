#include <vector>

#include <sweater/RangeTree/RTZ.hpp>

#include "gtest/gtest.h"

class BasicRTZTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
      points = {
        Point(1, 2, 3),
        Point(3, 2, 2),
        Point(5, 2, 7),
        Point(2, 6, 9),
        Point(2, 4, 9),
        Point(6, 3, 1.4),
        Point(6, 2, 1.4),
        Point(6, 3, 1),
        Point(4, 7, 5),
        Point(3, 1, 11),
      };

    }

    std::vector<Point> points;
};

TEST_F(BasicRTZTest, LowerBound1) {
  RTZ rtz(points.begin(), points.end());
  std::vector<Point>::iterator ub = 
    rtz.lowerBound(Point(30, 10, 0));

  ASSERT_EQ(*ub, Point(6, 3, 1));
}

TEST_F(BasicRTZTest, LowerBound2) {
  RTZ rtz(points.begin(), points.end());

  std::vector<Point>::iterator ub = 
    rtz.lowerBound(Point(4, 3, 4));

  ASSERT_EQ(*ub, Point(4, 7, 5));
}

TEST_F(BasicRTZTest, LowerBound3) {
  RTZ rtz(points.begin(), points.end());

  std::vector<Point>::iterator ub = 
    rtz.lowerBound(Point(4, 3, 2));

  ASSERT_EQ(*ub, Point(3, 2, 2));
}

TEST_F(BasicRTZTest, UpperBound1) {
  RTZ rtz(points.begin(), points.end());

  std::vector<Point>::iterator lb = 
    rtz.upperBound(Point(4, 3, 4));

  ASSERT_EQ(*lb, Point(1, 2, 3));
}

TEST_F(BasicRTZTest, UpperBound2) {
  RTZ rtz(points.begin(), points.end());

  std::vector<Point>::iterator lb = 
    rtz.upperBound(Point(4, 3, 2));

  ASSERT_EQ(*lb, Point(3, 2, 2));
}

TEST_F(BasicRTZTest, UpperBound3) {
  RTZ rtz(points.begin(), points.end());

  std::vector<Point>::iterator lb = 
    rtz.upperBound(Point(4, 3, 13));

  ASSERT_EQ(*lb, Point(3, 1, 11));
}

TEST_F(BasicRTZTest, find) {
  RTZ rtz(points.begin(), points.end());

  Point lbP(2, 1, 1.4);
  Point ubP(2, 1, 14);

  std::vector<Point>::iterator lb = rtz.lowerBound(lbP);
  std::vector<Point>::iterator ub = rtz.upperBound(ubP);

  std::vector<Point> points;

  rtz.find(lb, ub, points);

  ASSERT_EQ(points.size(), 9);

  ASSERT_NEAR(points[0][2], 1.4, 0.00001);
  ASSERT_NEAR(points[1][2], 1.4, 0.00001);
  ASSERT_EQ(points[2], Point(3, 2, 2));
  ASSERT_EQ(points[8], Point(3, 1, 11));
}

TEST_F(BasicRTZTest, find2) {
  RTZ rtz(points.begin(), points.end());

  Point lbP(2, 1, 1.5);
  Point ubP(2, 1, 9);

  std::vector<Point>::iterator lb = rtz.lowerBound(lbP);
  std::vector<Point>::iterator ub = rtz.upperBound(ubP);

  std::vector<Point> points;


  rtz.find(lb, ub, points);

  ASSERT_EQ(points.size(), 6);

  ASSERT_EQ(points[0], Point(3, 2, 2));
  ASSERT_EQ(points[3], Point(5, 2, 7));
  ASSERT_EQ(points[4][2], 9);
  ASSERT_EQ(points[5][2], 9);
}

TEST_F(BasicRTZTest, index) {
  RTZ rtz(points.begin(), points.end());

  Point lbP(2, 1, -100);
  std::vector<Point>::iterator lb = rtz.lowerBound(lbP);

  ASSERT_EQ(*lb, Point(6, 3, 1));

  ASSERT_EQ(rtz.getIndex(lb), 0);
}

TEST_F(BasicRTZTest, index2) {
  RTZ rtz(points.begin(), points.end());

  Point lbP(2, 1, 1.5);
  std::vector<Point>::iterator lb = rtz.lowerBound(lbP);

  ASSERT_EQ(*lb, Point(3, 2, 2));

  ASSERT_EQ(rtz.getIndex(lb), 3);
}

TEST_F(BasicRTZTest, maxMin) {
  RTZ rtz(points.begin(), points.end());

  ASSERT_EQ(rtz.getMin(), Point(6, 3, 1));
  ASSERT_EQ(rtz.getMax(), Point(3, 1, 11));
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
