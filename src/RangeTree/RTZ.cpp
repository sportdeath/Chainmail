#include "sweater/RangeTree/RTZ.hpp"

bool RTZ::lessThan(const Point & e1, const Point & e2) {
  return e1[2] < e2[2];
}

bool RTZ::equals(const Point & e1, const Point & e2) {
  return e1[2] == e2[2];
}
