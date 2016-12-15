#ifndef RTX_HPP
#define RTX_HPP

#include "sweater/RangeTree/RTY.hpp"
#include "sweater/RangeTree/BinaryRT1.hpp"

class RTX : public BinaryRT1<Point, RTY, RTX> {
  public:
    using BinaryRT1<Point, RTY, RTX>::BinaryRT1;

    static bool lessThan(const Point & e1, const Point & e2);
    static bool equals(const Point & e1, const Point & e2);
};

#endif
