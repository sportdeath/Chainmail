#ifndef BINARYRT0_HPP
#define BINARYRT0_HPP

#include "sweater/RangeTree/BinaryRT.hpp"

/**
 * Node is an ArrayRT<T>
 */
template <typename T, typename Node, typename Derived>
class BinaryRT0 : public BinaryRT<T, Node, Derived> {
  public:
    typedef typename std::vector<T>::iterator iterator;
    
    BinaryRT0(){};

    /**
     * Each constructor calls super class constructor
     * and then initializes node
     * Recursively calls child one.
     */
    BinaryRT0 (
        iterator begin, 
        iterator end,
        bool sorted = false,
        bool assignNode = true);

    /**
     * Splits a node into
     * left and right node
     */
    void splitNode();

    static iterator partition(
        iterator begin,
        iterator end,
        const T & splitPoint
        );

    /**
      * Assigns node and recursively
      * calls assign node on children
      */
    void assignChildNodes();

    /**
     * Finds node iterators and calls child constructor
     */
    void find(const T & e1, const T & e2, std::vector<T> & results);

    /**
     * Assignment operator.
     */
    using BinaryRT<T, Node, Derived>::operator=;

  private:
    /**
     * e1 <= e2
     * 
     * nodeBegin is the first element node search range
     * nodeEnd is the last element in the node search range
     */
    void find(
        const T & e1, 
        const T & e2, 
        iterator nodeBegin, 
        iterator nodeEnd,
        std::vector<T> & results);

    /**
     *
     * left = true iff we are searching left subtrees
     */
    void findSubtrees(
        const T & e, 
        iterator nodeBegin, 
        iterator nodeEnd, 
        bool left,
        std::vector<T> & results);
};

#include "../../../src/RangeTree/BinaryRT0.cpp"

#endif
