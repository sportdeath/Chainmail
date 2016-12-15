#include "sweater/RangeTree/RTY.hpp"

bool RTY::lessThan(const Point & e1, const Point & e2) {
  return e1[1] < e2[1];
}

bool RTY::equals(const Point & e1, const Point & e2) {
  return e1[1] == e2[1];
}
