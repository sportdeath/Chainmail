#ifndef BINARYRT1_HPP
#define BINARYRT1_HPP

#include <vector>

#include "sweater/RangeTree/BinaryRT0.hpp"
#include "sweater/RangeTree/BinaryRT.hpp"

/**
 * Node is an BinaryRT0<T>
 */
template <typename T, typename Node, typename Derived>
class BinaryRT1 : public BinaryRT<T, Node, Derived> {
  public:
    typedef typename std::vector<T>::iterator iterator;
    
    /**
     * Each constructor calls super class constructor
     */
    BinaryRT1 (
        iterator begin, 
        iterator end,
        bool sorted = false);

    BinaryRT1(const BinaryRT1 & that) 
      : BinaryRT<T, Node, Derived>(that) {}

    void find(const T & e1, const T & e2, std::vector<T> & results);

    // requires e1 <= e2
    void findSorted(const T & e1, const T & e2, std::vector<T> & results);

    /**
     * Assignment operator.
     */
    using BinaryRT<T, Node, Derived>::operator=;


  private:
    void findSubtrees(const T & e1, const T & e2, bool left, std::vector<T> & results);
};

#include "../../../src/RangeTree/BinaryRT1.cpp"

#endif
