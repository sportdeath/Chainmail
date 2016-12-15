#include <vector>
#include <iostream>

#include "sweater/RangeTree/RT.hpp"

template <typename T, typename Derived>
bool RT<T, Derived>::isBound(const T & e, bool left) const {
  if (left) {
    // e1 <= min
    // min >= e1
    return
      not Derived::lessThan(getMin(), e);
  } else {
    // e >= max
    return
      not Derived::lessThan(e, getMax());
  }
}

template <typename T, typename Derived>
bool RT<T, Derived>::intersects(const T & e1, const T & e2) const {
  // e1 <= max
  // and
  // e2 >= min
  return 
    (not Derived::lessThan(getMax(), e1))
    and
    (not Derived::lessThan(e2, getMin()));
}

template <typename T, typename Derived>
typename std::vector<T>::iterator RT<T, Derived>::getLargestEqualIndex(const typename std::vector<T>::iterator it, const typename std::vector<T>::iterator end) {
  typename std::vector<T>::iterator out = it;
  while ((out + 1 < end) && Derived::equals(*out, *(out + 1))) {
    out++;
  }
  return out;
}

template <typename T, typename Derived>
typename std::vector<T>::iterator RT<T, Derived>::getSmallestEqualIndex(
    const typename std::vector<T>::iterator it, 
    const typename std::vector<T>::iterator begin
    ) {
  typename std::vector<T>::iterator out = it;
  while ((out > begin) && Derived::equals(*out, *(out - 1))) {
    out --;
  }
  return out;
}
