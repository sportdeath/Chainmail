#include "sweater/RangeTree/RTX.hpp"

bool RTX::lessThan(const Point & e1, const Point & e2) {
  return e1[0] < e2[0];
}

bool RTX::equals(const Point & e1, const Point & e2) {
  return e1[0] == e2[0];
}
