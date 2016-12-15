#ifndef RTZ_HPP
#define RTZ_HPP

#include <eigen3/Eigen/Dense>

#include "sweater/RangeTree/RT.hpp"
#include "sweater/RangeTree/ArrayRT.hpp"

typedef Eigen::Vector3f Point;

class RTZ : public ArrayRT<Point, RTZ> {
  public:
    using ArrayRT<Point, RTZ>::ArrayRT;

    static bool lessThan(const Point & e1, const Point & e2);
    static bool equals(const Point & e1, const Point & e2);
};

#endif
