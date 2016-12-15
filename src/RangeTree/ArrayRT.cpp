#include <iostream>

#include "sweater/RangeTree/ArrayRT.hpp"

template <typename T, typename Derived>
ArrayRT<T, Derived>::ArrayRT(
    iterator begin,
    iterator end,
    bool sorted
    ) {
  // copy to internal vector
  data.resize(end - begin);

  std::move(begin, end, this -> begin());

  // sort
  if (not sorted) {
    std::stable_sort(data.begin(), data.end(), Derived::lessThan);
  }

  // Child pointers
  leftSuccessorChildren.resize(data.size());
  rightSuccessorChildren.resize(data.size());
  leftPredecessorChildren.resize(data.size());
  rightPredecessorChildren.resize(data.size());
}

template <typename T, typename Derived>
void ArrayRT<T, Derived>::find(iterator start, iterator end, std::vector<T> & results) const {
  if (
      (start > end)
      or
      (end >= data.end())
      or
      (start < data.begin())
      ) return;
  assert(start >= data.begin());
  assert(end < data.end());
  results.insert(results.end(), start, end + 1);
}

template <typename T, typename Derived>
typename std::vector<T>::iterator ArrayRT<T, Derived>::begin() {
  return data.begin();
}

template <typename T, typename Derived>
typename std::vector<T>::iterator ArrayRT<T, Derived>::end() {
  return data.end();
}

// <---|
template <typename T, typename Derived>
typename std::vector<T>::iterator ArrayRT<T, Derived>::upperBound(const T & e) {
  iterator ub = std::upper_bound(begin(), end(), e, Derived::lessThan);
  while ((ub >= end()) || ((ub > begin()) && Derived::lessThan(e, *ub))) {
    ub--;
  }
  return ub;
}

// |--->
template <typename T, typename Derived>
typename std::vector<T>::iterator ArrayRT<T, Derived>::lowerBound(const T & e) {
  return std::lower_bound(begin(), end(), e, Derived::lessThan);
}

template <typename T, typename Derived>
const T & ArrayRT<T, Derived>::getMin() const {
  return data.front();
}

template <typename T, typename Derived>
const T & ArrayRT<T, Derived>::getMax() const {
  return data.back();
}

template <typename T, typename Derived>
int ArrayRT<T, Derived>::size() const {
  return data.size();
}

template <typename T, typename Derived>
int ArrayRT<T, Derived>::getIndex(iterator it) const {
  return it - data.begin();
}

template <typename T, typename Derived>
typename std::vector<T>::iterator ArrayRT<T, Derived>::getSuccessorChild(iterator parent, bool left) const {
  int index = getIndex(parent);

  if (left) {
    return leftSuccessorChildren[index];
  } else {
    return rightSuccessorChildren[index];
  }
}

template <typename T, typename Derived>
typename std::vector<T>::iterator ArrayRT<T, Derived>::getPredecessorChild(
    iterator parent, 
    bool left) const {

  int index = getIndex(parent);

  if (left) {
    return leftPredecessorChildren[index];
  } else {
    return rightPredecessorChildren[index];
  }

}

template <typename T, typename Derived>
void ArrayRT<T, Derived>::setSuccessorChild(iterator parent, iterator child, bool left) {
  int index = getIndex(parent);

  if (left) {
    leftSuccessorChildren[index] = child;
  } else {
    rightSuccessorChildren[index] = child;
  }
}

template <typename T, typename Derived>
void ArrayRT<T, Derived>::setPredecessorChild(iterator parent, iterator child, bool left) {
  int index = getIndex(parent);

  if (left) {
    leftPredecessorChildren[index] = child;
  } else {
    rightPredecessorChildren[index] = child;
  }
}
