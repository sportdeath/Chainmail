#ifndef RTY_HPP
#define RTY_HPP

#include "sweater/RangeTree/RTZ.hpp"
#include "sweater/RangeTree/BinaryRT0.hpp"

class RTY : public BinaryRT0<Point, RTZ, RTY> {
  public:
    using BinaryRT0<Point, RTZ, RTY>::BinaryRT0;

    static bool lessThan(const Point & e1, const Point & e2);
    static bool equals(const Point & e1, const Point & e2);
};

#endif
