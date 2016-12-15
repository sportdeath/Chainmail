#include <vector>

#include <sweater/RangeTree/RTY.hpp>

#include "gtest/gtest.h"

class BasicRTYTest : public ::testing::Test {
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

    static bool contains(std::vector<Point> vec, Point x) {

      return std::find(vec.begin(), vec.end(), x) != vec.end();

    }

    std::vector<Point> points;
};

TEST_F(BasicRTYTest, Init) {
  RTY rty(points.begin(), points.end());
}

TEST_F(BasicRTYTest, minMax) {
  RTY rty(points.begin(), points.end());

  ASSERT_EQ(rty.getMax(), Point(4, 7, 5));
  ASSERT_EQ(rty.getMin(), Point(3, 1, 11));
}

TEST_F(BasicRTYTest, successorPredecessor) {
  Point a(0,1,2);
  Point b(0,2,1);
  Point c(0,3,2);

  std::vector<Point> testPoints = {a,b,c};

  RTY rty(testPoints.begin(), testPoints.end());

  std::vector<Point>::iterator it = rty.getNode() -> begin();

  std::vector<Point>::iterator result;
  // left (children are a and b)
  ASSERT_EQ(*it, b);
  result = rty.getNode() -> getSuccessorChild(it, true);
  ASSERT_EQ(*result, b);

  it++;
  ASSERT_EQ((*it)[2], 2);
  result = rty.getNode() -> getSuccessorChild(it, true);
  ASSERT_EQ(*result, a);

  it++;
  ASSERT_EQ((*it)[2], 2);
  result = rty.getNode() -> getSuccessorChild(it, true);
  ASSERT_EQ(*result, a);
  

  it = rty.getNode() -> begin();
  // right (children: simply c)
  result = rty.getNode() -> getSuccessorChild(it, false);
  ASSERT_EQ(*result, c);
  it++;
  result = rty.getNode() -> getSuccessorChild(it, false);
  ASSERT_EQ(*result, c);
  it++;
  result = rty.getNode() -> getSuccessorChild(it, false);
  ASSERT_EQ(*result, c);

  // Predecessor!
  it = rty.getNode() -> begin();
  // left, a and b
  result = rty.getNode() -> getPredecessorChild(it, true);
  ASSERT_EQ(*result, b);
  it++;
  result = rty.getNode() -> getSuccessorChild(it, true);
  ASSERT_EQ(*result, a);
  it++;
  result = rty.getNode() -> getSuccessorChild(it, true);
  ASSERT_EQ(*result, a);

  it = rty.getNode() -> begin();
  // right: c
  it++; // b does not have a predecessor
  result = rty.getNode() -> getPredecessorChild(it, false);
  ASSERT_EQ(*result, c);
  it++;
  result = rty.getNode() -> getSuccessorChild(it, false);
  ASSERT_EQ(*result, c);
}

TEST_F(BasicRTYTest, succPred) {
  RTY rty(points.begin(), points.end());

  Point small(3, 2, 4);
  Point large(3, 7, 9);

  std::vector<Point>::iterator lb = rty.getNode() -> lowerBound(small);
  std::vector<Point>::iterator ub = rty.getNode() -> upperBound(large);

  ASSERT_EQ(*lb, Point(4, 7, 5));
  ASSERT_EQ((*ub)[2], 9);
  ASSERT_EQ(ub - lb + 1, 4);

  // left
  std::vector<Point>::iterator leftS = rty.getNode() -> getSuccessorChild(lb, true);
  ASSERT_EQ(*leftS, Point(5, 2, 7));

  std::vector<Point>::iterator leftP = rty.getNode() -> getPredecessorChild(ub, true);
  ASSERT_EQ(*leftP, Point(5, 2, 7));

  // right
  std::vector<Point>::iterator rightS = rty.getNode() -> getSuccessorChild(lb, false);
  ASSERT_EQ(*rightS, Point(4, 7, 5));

  std::vector<Point>::iterator rightP = rty.getNode() -> getPredecessorChild(ub, false);
  ASSERT_EQ((*rightP)[2], 9);
}

TEST_F(BasicRTYTest, findInfiniteYBound) {
  RTY rty(points.begin(), points.end());

  std::vector<Point>::iterator x = rty.getNode() -> begin();
  Point testPoint(2, 6, 9);

  while (*x != testPoint) x++;

  ASSERT_EQ(*x, testPoint);

  std::vector<Point>::iterator predOfTest 
    = rty.getNode() -> getPredecessorChild(x, false);

  ASSERT_EQ(*predOfTest, testPoint);

  Point min(3, 10000, 4);
  Point max(3, -10000, 9);

  std::vector<Point> results;
  rty.find(min, max, results);

  ASSERT_EQ(results.size(), 4);
  ASSERT_TRUE(contains(results, Point(5, 2, 7)));
  ASSERT_TRUE(contains(results, Point(2, 6, 9)));
  ASSERT_TRUE(contains(results, Point(2, 4, 9)));
  ASSERT_TRUE(contains(results, Point(4, 7, 5)));
}

TEST_F(BasicRTYTest, succPred2) {
  RTY rty(points.begin(), points.end());

  Point min(3, 4, 4);
  Point max(3, 6, 9);

  std::vector<Point>::iterator lb = rty.getNode() -> lowerBound(min);
  std::vector<Point>::iterator ub = rty.getNode() -> upperBound(max);

  ASSERT_EQ(*lb, Point(4, 7, 5));
  ASSERT_EQ((*ub)[2], 9);
  ASSERT_EQ(ub - lb + 1, 4);

  // right
  std::vector<Point>::iterator rightS = rty.getNode() -> getSuccessorChild(lb, false);
  ASSERT_EQ(*rightS, Point(4, 7, 5));

  std::vector<Point>::iterator rightP = rty.getNode() -> getPredecessorChild(ub, false);
  ASSERT_EQ((*rightP)[2], 9);

  // left of right child
  RTY * rightChild = rty.getChild(false);
  std::vector<Point>::iterator leftS = rightChild -> getNode() -> getSuccessorChild(rightS, true);
  ASSERT_EQ(*leftS, Point(2, 4, 9));


  // Check this sheet


  std::vector<Point>::iterator leftP = rightChild -> getNode() -> getPredecessorChild(rightP, true);
  ASSERT_EQ(*leftP, Point(2, 4, 9));
}

TEST_F(BasicRTYTest, tightYBound) {
  RTY rty(points.begin(), points.end());

  Point min(3, 2, 4);
  Point max(3, 7, 9);

  std::vector<Point> results;
  rty.find(min, max, results);

  ASSERT_EQ(results.size(), 4);
  ASSERT_TRUE(contains(results, Point(5, 2, 7)));
  ASSERT_TRUE(contains(results, Point(2, 6, 9)));
  ASSERT_TRUE(contains(results, Point(2, 4, 9)));
  ASSERT_TRUE(contains(results, Point(4, 7, 5)));
}

TEST_F(BasicRTYTest, smallYBound) {
  RTY rty(points.begin(), points.end());

  Point min(3, 4, 4);
  Point max(3, 6, 9);

  std::vector<Point> results;
  rty.find(min, max, results);

  ASSERT_EQ(results.size(), 2);
  ASSERT_TRUE(contains(results, Point(2, 6, 9)));
  ASSERT_TRUE(contains(results, Point(2, 4, 9)));
}

TEST_F(BasicRTYTest, doubleChildren) {
  RTY rty(points.begin(), points.end());

  RTY * leftLeftChild = rty.getChild(true) -> getChild(true);

  RTY * lChild = leftLeftChild -> getChild(true);
  RTY * rChild = leftLeftChild -> getChild(false);


  std::vector<Point>::iterator evilPoint = leftLeftChild -> getNode() -> end() - 1;

  ASSERT_EQ(*evilPoint, Point(3, 1, 11));

  std::vector<Point>::iterator lp = leftLeftChild
    -> getNode() -> getPredecessorChild(evilPoint, true);

  std::vector<Point>::iterator rp = leftLeftChild
    -> getNode() -> getPredecessorChild(evilPoint, false);

  ASSERT_EQ(*lp, Point(3, 1, 11));
  ASSERT_EQ(*rp, Point(3, 2, 2));

}

TEST_F(BasicRTYTest, infiniteZBound) {
  RTY rty(points.begin(), points.end());

  Point min(3, 2, -1000);
  Point max(3, 6, 1000);

  std::vector<Point> results;
  rty.find(min, max, results);

  ASSERT_EQ(results.size(), 8);
  ASSERT_TRUE(contains(results, Point(1, 2, 3)));
  ASSERT_TRUE(contains(results, Point(3, 2, 2)));
  ASSERT_TRUE(contains(results, Point(5, 2, 7)));
  ASSERT_TRUE(contains(results, Point(2, 6, 9)));
  ASSERT_TRUE(contains(results, Point(2, 4, 9)));
  ASSERT_TRUE(contains(results, Point(6, 3, 1.4)));
  ASSERT_TRUE(contains(results, Point(6, 2, 1.4)));
  ASSERT_TRUE(contains(results, Point(6, 3, 1)));
}


int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
