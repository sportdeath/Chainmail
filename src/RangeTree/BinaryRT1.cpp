#include <iostream>

#include "sweater/RangeTree/BinaryRT.hpp"
#include "sweater/RangeTree/BinaryRT1.hpp"

template <typename T, typename Node, typename Derived>
BinaryRT1<T, Node, Derived>::BinaryRT1 (
    iterator begin, 
    iterator end,
    bool sorted) 
  : BinaryRT<T, Node, Derived>(begin, end, sorted)
{
  if ((end - begin) == 1) {
    this -> leftChild = nullptr;
    this -> rightChild = nullptr;
  } else {
    // find middle
    iterator middle = this -> rightMin(begin, end);

    assert((middle - begin) - (end - middle) >= 0);
    assert((middle - begin) - (end - middle) <= 1);

    this -> leftChild = new Derived(begin, middle, true);
    this -> rightChild = new Derived(middle, end, true);
  }

  std::vector<T> childArray;
  childArray.assign(begin, end);
  this -> node = new Node(childArray.begin(), childArray.end());
}

template <typename T, typename Node, typename Derived>
void BinaryRT1<T, Node, Derived>::find(
    const T & e1, 
    const T & e2,
    std::vector<T> & results
    ) {
  T smaller, bigger;
  if (Derived::lessThan(e1, e2)) {
    smaller = e1;
    bigger = e2;
  } else {
    bigger = e1;
    smaller = e2;
  }

  findSorted(e1, e2, results);
}

template <typename T, typename Node, typename Derived>
void BinaryRT1<T, Node, Derived>::findSorted(
    const T & e1, 
    const T & e2,
    std::vector<T> & results
    ) {

  // if no intersection, simply return
  if (not this -> intersects(e1, e2)) {
    return;
  }

  // if at at bottom of tree return findings
  if (this -> getChild(false) == nullptr) {
    this -> getNode() -> find(e1, e2, results);
    return;
  }

  bool e1Direction = this -> chooseBranch(e1, true);
  bool e2Direction = this -> chooseBranch(e2, false);

  if (e1Direction == e2Direction) {
    // Continue
    this -> getChild(e1Direction) -> findSorted(e1, e2, results);
  } else {
    // Add from both subtrees
    this -> getChild(e1Direction) -> findSubtrees(e1, e2, not e1Direction, results);
    this -> getChild(e2Direction) -> findSubtrees(e2, e1, not e2Direction, results);
  }
}

template <typename T, typename Node, typename Derived>
void BinaryRT1<T, Node, Derived>::findSubtrees(
    const T & e1, 
    const T & e2, 
    bool left, 
    std::vector<T> & results
    ) {

  if (this -> isBound(e1, not left)) {
    this -> getNode() -> find(e1, e2, results);
    return;
  }

  if (this -> getChild(false) == nullptr) return;
  
  bool e1Direction = this -> chooseBranch(e1, not left);

  // continue searching
  this -> getChild(e1Direction) -> findSubtrees(e1, e2, left, results);

  if (e1Direction != left) {
    this -> getChild(left) -> getNode() -> find(e1, e2, results);
  }
}
