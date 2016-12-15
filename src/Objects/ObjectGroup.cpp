#include "sweater/Objects/ObjectGroup.hpp"

bool ObjectGroup::intersect(
    const Ray & ray,
    float tmin,
    Hit & hit) {

  bool didIntersect = false;

  for (auto it = begin(); it < end(); it++) {
    didIntersect |= (*it) -> intersect(ray, tmin, hit);
  }

  return didIntersect;
}

void ObjectVector::computeSelfInteraction() {
  for (Object * object1 : objects) {
    for (Object * object2 : objects) {
      if (object1 != object2) {
        object1 -> collide(object2);
      }
    }
  }
}
