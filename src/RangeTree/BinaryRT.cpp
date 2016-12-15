#include <cassert>

#include "sweater/RangeTree/BinaryRT.hpp"

template <typename T, typename Node, typename Derived>
BinaryRT<T, Node, Derived>::BinaryRT (
    iterator begin, 
    iterator end,
    bool sorted) {

  assert(end - begin > 0);

  if (not sorted) {
    std::stable_sort(begin, end, Derived::lessThan);
  }

  min = *begin;
  max = *(end - 1);
}

template <typename T, typename Node, typename Derived>
BinaryRT<T, Node, Derived>::~BinaryRT() {
  delete leftChild;
  leftChild = nullptr;
  delete rightChild;
  rightChild = nullptr;
  delete node;
  node = nullptr;
}

template <typename T, typename Node, typename Derived>
Node * BinaryRT<T, Node, Derived>::getNode() {
  return node;
}

template <typename T, typename Node, typename Derived>
Derived * BinaryRT<T, Node, Derived>::getChild(bool left) {
  if (left) {
    return leftChild;
  } else {
    return rightChild;
  }
}

template <typename T, typename Node, typename Derived>
const T & BinaryRT<T, Node, Derived>::getMax() const {
  return max;
}

template <typename T, typename Node, typename Derived>
const T & BinaryRT<T, Node, Derived>::getMin() const {
  return min;
}

template <typename T, typename Node, typename Derived>
bool BinaryRT<T, Node, Derived>::chooseBranch(const T & e, bool left) const {
  // if max of left child and min of left child are equal...
  // then if left (e is a lower bound)
  // we choose the left branch
  // otherwise we choose the right branch

  if (Derived::equals(e, rightChild -> getMin()) and
      Derived::equals(rightChild -> getMin(), leftChild -> getMax())) {
    return left;
  } else {
    return Derived::lessThan(e, rightChild -> getMin());
  }
}

template <typename T, typename Node, typename Derived>
typename std::vector<T>::iterator BinaryRT<T, Node, Derived>::rightMin(iterator begin, iterator end) {
  int rightSize = (end - begin)/2;
  return end - rightSize;

}
