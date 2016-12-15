#include "sweater/RangeTree/ObjectRangeTree.hpp"

bool ObjectRangeTreeX::lessThan(const Object * e1, const Object * e2) {
  return e1 -> trialState.position[0] < e2 -> trialState.position[0];
}

bool ObjectRangeTreeX::equals(const Object * e1, const Object * e2) {
  return e1 -> trialState.position[0] == e2 -> trialState.position[0];
}

bool ObjectRangeTreeY::lessThan(const Object * e1, const Object * e2) {
  return e1 -> trialState.position[1] < e2 -> trialState.position[1];
}

bool ObjectRangeTreeY::equals(const Object * e1, const Object * e2) {
  return e1 -> trialState.position[1] == e2 -> trialState.position[1];
}

bool ObjectRangeTreeZ::lessThan(const Object * e1, const Object * e2) {
  return e1 -> trialState.position[2] < e2 -> trialState.position[2];
}

bool ObjectRangeTreeZ::equals(const Object * e1, const Object * e2) {
  return e1 -> trialState.position[2] == e2 -> trialState.position[2];
}
