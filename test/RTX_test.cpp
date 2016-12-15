#include <vector>

#include <sweater/RangeTree/RTX.hpp>

#include "gtest/gtest.h"

class BasicRTXTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
      points = {
        Point(1, 2, 3),
        Point(3, 2, 2),
        Point(5, 2, 7),
        Point(2, 6, 9),
        Point(2, 4, 9),
        Point(6, 3, 1.4),
        Point(22, 2, 1.4),
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

TEST_F(BasicRTXTest, SingleInit) {
  std::vector<Point> singlePoint = {
    Point(1,2,3),
    Point(3,2,2)
  };
  RTX rtx(singlePoint.begin(), singlePoint.end());
}

TEST_F(BasicRTXTest, Init) {
  RTX rtx(points.begin(), points.end());
}

TEST_F(BasicRTXTest, minMax) {
  RTX rtx(points.begin(), points.end());

  ASSERT_EQ(rtx.getMax(), Point(22, 2, 1.4));
  ASSERT_EQ(rtx.getMin(), Point(1, 2, 3));
}

TEST_F(BasicRTXTest, InfiniteXRange) {
  RTX rtx(points.begin(), points.end());

  Point min(-1000, 2, 4);
  Point max(1000, 7, 9);

  std::vector<Point> results;
  rtx.find(min, max, results);

  ASSERT_EQ(results.size(), 4);
  ASSERT_TRUE(contains(results, Point(5, 2, 7)));
  ASSERT_TRUE(contains(results, Point(2, 6, 9)));
  ASSERT_TRUE(contains(results, Point(2, 4, 9)));
  ASSERT_TRUE(contains(results, Point(4, 7, 5)));
}

TEST_F(BasicRTXTest, chooseBranch) {
  RTX rtx(points.begin(), points.end());

  Point point(4, 2, 4);

  RTX * rightChild = rtx.getChild(false);

  ASSERT_EQ(rightChild -> getNode() -> getNode() -> size(), 5);

  bool directionOfTravel = rightChild -> chooseBranch(point, false);

  ASSERT_EQ(directionOfTravel, true); // going left
}

TEST_F(BasicRTXTest, TightXRange) {
  RTX rtx(points.begin(), points.end());

  Point min(4, 2, 4);
  Point max(2, 7, 9);

  std::vector<Point> results;
  rtx.find(min, max, results);

  ASSERT_EQ(results.size(), 3);
  ASSERT_TRUE(contains(results, Point(2, 6, 9)));
  ASSERT_TRUE(contains(results, Point(2, 4, 9)));
  ASSERT_TRUE(contains(results, Point(4, 7, 5)));
}

//TEST(RandomTestingRT, RandomTest) {
  //for (int i = 0; i < 10; i++) {
    //int pointSize;
    //std::vector<Point> points;

    //RTX rtx(points.begin(), points.end());
  //}
//}



int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
