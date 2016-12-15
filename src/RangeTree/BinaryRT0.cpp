#include <iostream>
#include "sweater/RangeTree/BinaryRT0.hpp"

template <typename T, typename Node, typename Derived>
BinaryRT0<T, Node, Derived>::BinaryRT0 (
    iterator begin, 
    iterator end,
    bool sorted,
    bool assignNode) 
  : BinaryRT<T, Node, Derived>(begin, end, sorted)
{
  if ((end - begin) == 1) {

    this -> leftChild = nullptr;
    this -> rightChild = nullptr;

  } else {
    // find middle (its already sorted!)
    iterator middle = this -> rightMin(begin, end);

    assert((middle - begin) - (end - middle) >= 0);
    assert((middle - begin) - (end - middle) <= 1);

    // make left and right children
    this -> leftChild = new Derived(begin, middle, true, false);
    this -> rightChild = new Derived(middle, end, true, false);
  }

  // get and assign their nodes
  // using node split
  // (this is recursive)
  // assign node
  if (assignNode) {
    this -> node = new Node(begin, end);
    assignChildNodes();
  }
}

template <typename T, typename Node, typename Derived>
typename std::vector<T>::iterator BinaryRT0<T, Node, Derived>::partition(
    iterator begin,
    iterator end, 
    const T & splitPoint) {

  // count number of elements which are strictly less
  int numStrictLess = 0;
  for (iterator it = begin; it < end; it++) {
    if (Derived::lessThan(*it, splitPoint)) {
      numStrictLess++;
    }
  }

  // The number of elements in first half which must be filled
  // by equal elements
  iterator middle = BinaryRT0<T, Node, Derived>::rightMin(begin, end);
  int numToFill = (middle - begin) - numStrictLess;

  // Allocate swap array
  std::vector<T> swapArray(end - begin);
  iterator first = swapArray.begin();
  iterator second = BinaryRT0<T, Node, Derived>::rightMin(swapArray.begin(), swapArray.end());

  // Sort into first or second half
  for (iterator it = begin; it < end; it++) {
    if (Derived::equals(*it, splitPoint) and numToFill > 0) {
      // put in first
      *first = *it;
      first++;
      numToFill--;
    } else {
       if (Derived::lessThan(*it, splitPoint)) {
         // put in first
         *first = *it;
         first++;
       } else {
         // put in second
         *second = *it;
         second++;
       }
    } 
  }

  // move and deallocate
  std::move(swapArray.begin(), swapArray.end(), begin);

  return middle;

}

template <typename T, typename Node, typename Derived>
void BinaryRT0<T, Node, Derived>::splitNode() {
  // copy vector over
  std::vector<T> nodeChildren;
  nodeChildren.assign(this -> getNode() -> begin(), this -> getNode() -> end());

  // partition by parent sorter and split point
  T splitPoint = this -> getChild(false) -> getMin(); // rightChild min

  iterator middle = partition(
      nodeChildren.begin(), 
      nodeChildren.end(), 
      splitPoint);

  // create left and right children
  this -> leftChild -> node = new Node(nodeChildren.begin(), middle, true);
  this -> rightChild -> node = new Node(middle, nodeChildren.end(), true);

  Node * leftNode = this -> leftChild -> getNode();
  Node * rightNode = this -> rightChild -> getNode();
  // Assign children
  iterator leftNodePointer = leftNode -> begin();
  iterator rightNodePointer = rightNode -> begin();

  // Successor
  for (iterator it = this -> getNode() -> begin(); 
      it < this -> getNode() -> end(); 
      it++) {
    this -> getNode() -> setSuccessorChild(it, leftNodePointer, true);
    this -> getNode() -> setSuccessorChild(it, rightNodePointer, false);

    assert((not Node::lessThan(*leftNodePointer, *it)) or leftNodePointer >= leftNode -> end());
    assert((not Node::lessThan(*rightNodePointer, *it)) or rightNodePointer >= rightNode -> end());

    if (it + 1 < this -> getNode() -> end()) {
      if (not Node::equals(*it, *(it + 1))) {
        if ((leftNodePointer < leftNode -> end()) &&
              Node::equals(*it, *leftNodePointer)) {
          leftNodePointer = 
            Node::getLargestEqualIndex(
                leftNodePointer,
                leftNode -> end()
                ) + 1;
        } 
        if ((rightNodePointer < rightNode -> end()) &&
          Node::equals(*it, *rightNodePointer)) {
          rightNodePointer = 
            Node::getLargestEqualIndex(
                rightNodePointer,
                rightNode -> end()
                ) + 1;
        }
      }
    }
  }


  leftNodePointer = leftNode -> end() - 1;
  rightNodePointer = rightNode -> end() - 1;

  // Predecessor
  for (iterator it = this -> getNode() -> end() - 1; 
      it >= this -> getNode() -> begin(); 
      it--) {
    this -> getNode() -> setPredecessorChild(it, leftNodePointer, true);
    this -> getNode() -> setPredecessorChild(it, rightNodePointer, false);

    if (it > this -> getNode() -> begin()) {
      if (not Node::equals(*it, *(it - 1))) {
        if ((leftNodePointer >= leftNode -> begin()) &&
            Node::equals(*it, *leftNodePointer)) {
          leftNodePointer = 
            Node::getSmallestEqualIndex(
                leftNodePointer, 
                leftNode -> begin()) - 1;
        } 
        if ((rightNodePointer >= rightNode -> begin()) &&
            Node::equals(*it, *rightNodePointer)) {
          rightNodePointer = 
            Node::getSmallestEqualIndex(
                rightNodePointer, 
                rightNode -> begin()) - 1;
        }
      }
    }
  }
}

template <typename T, typename Node, typename Derived>
void BinaryRT0<T, Node, Derived>::assignChildNodes() {
  if (this -> getNode() -> size() == 1) return;

  // assigns left and right children nodes
  splitNode();
  this -> leftChild -> assignChildNodes();
  this -> rightChild -> assignChildNodes();
}

/**
 * Finds node iterators and calls child constructor
 */
template <typename T, typename Node, typename Derived>
void BinaryRT0<T, Node, Derived>::find(const T & e1, const T & e2, std::vector<T> & results) {

  // find smaller and bigger
  T smaller = std::min(e1, e2, Node::lessThan);
  T bigger = std::max(e1, e2, Node::lessThan);

  iterator nodeBegin = this -> getNode() -> lowerBound(smaller);
  iterator nodeEnd = this -> getNode() -> upperBound(bigger);

  smaller = std::min(e1, e2, Derived::lessThan);
  bigger = std::max(e1, e2, Derived::lessThan);

  find(smaller, bigger, nodeBegin, nodeEnd, results);
}


// guarenteed that e1 <= e2
template <typename T, typename Node, typename Derived>
void BinaryRT0<T, Node, Derived>::find(
    const T & e1, 
    const T & e2, 
    iterator nodeBegin, 
    iterator nodeEnd,
    std::vector<T> & results) {

  // if tree does not contain search results
  if (not this -> intersects(e1, e2)) {
    return;
  }

  // if at end
  // right child will be null
  if (this -> getChild(false) == nullptr) {
    this -> getNode() -> find(nodeBegin, nodeEnd, results);
    return;
  }

  bool e1Direction = this -> chooseBranch(e1, true);
  bool e2Direction = this -> chooseBranch(e2, false);

  if (e1Direction == e2Direction) {

    // Continue
    this -> getChild(e1Direction) 
      -> find(
          e1, 
          e2, 
          this -> getNode() -> getSuccessorChild(nodeBegin, e1Direction),
          this -> getNode() -> getPredecessorChild(nodeEnd, e1Direction),
          results
          );
  } else {
    // Add from both subtrees
    this -> getChild(e1Direction) 
      -> findSubtrees(
          e1, 
          this -> getNode() -> getSuccessorChild(nodeBegin, e1Direction),
          this -> getNode() -> getPredecessorChild(nodeEnd, e1Direction),
          not e1Direction,
          results);

    this -> getChild(e2Direction) 
      -> findSubtrees(
          e2, 
          this -> getNode() -> getSuccessorChild(nodeBegin, e2Direction),
          this -> getNode() -> getPredecessorChild(nodeEnd, e2Direction),
          not e2Direction,
          results);
  }
}
template <typename T, typename Node, typename Derived>
void BinaryRT0<T, Node, Derived>::findSubtrees(
    const T & e, 
    iterator nodeBegin, 
    iterator nodeEnd, 
    bool left,
    std::vector<T> & results) {

  // if nothing in subnode 
  if (nodeBegin > nodeEnd) {
    return;
  }

  // if size = 1
  // or
  // if entire tree is contained
  // return it
  //if (this -> getNode() -> size() == 1 or this -> isBound(e, not left)) {
  if (this -> isBound(e, not left)) {
    this -> getNode() -> find(nodeBegin, nodeEnd, results);
    return;
  }

  bool eDirection = this -> chooseBranch(e, not left);

  // Continue searching
  this -> getChild(eDirection) 
    -> findSubtrees(
        e, 
        this -> getNode() -> getSuccessorChild(nodeBegin, eDirection),
        this -> getNode() -> getPredecessorChild(nodeEnd, eDirection),
        left,
        results);

  // Going in other direction
  if (eDirection != left) {
    this -> getChild(left) 
      -> getNode() -> find(
          this -> getNode() -> getSuccessorChild(nodeBegin, left),
          this -> getNode() -> getPredecessorChild(nodeEnd, left),
          results
          );
  }

}
