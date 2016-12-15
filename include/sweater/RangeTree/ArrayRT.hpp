#ifndef ARRAYRT_HPP
#define ARRAYRT_HPP

#include <vector>

#include "sweater/RangeTree/RT.hpp"
template <typename T, typename Derived> class ArrayRT : public RT<T, Derived> {
  public:
    typedef typename std::vector<T>::iterator iterator;

    ArrayRT(){};

    /**
     * Constructor 
     */
    ArrayRT(
        iterator begin,
        iterator end,
        bool sorted = false
        );

    void find(iterator start, iterator end, std::vector<T> & results) const;

    virtual const T & getMax() const;
    virtual const T & getMin() const;

    int size() const;
    int getIndex(iterator it) const;
    iterator begin();
    iterator end();
    iterator lowerBound(const T & e);
    iterator upperBound(const T & e);

    /**
     * Iterator into child arrays
     */
    // returns the sucessor
    // i.e. the first child that is >= the parent
    void setSuccessorChild(iterator parent, iterator child, bool left);
    void setPredecessorChild(iterator parent, iterator child, bool left);
    iterator getSuccessorChild(iterator parent, bool left) const;
    iterator getPredecessorChild(iterator parent, bool left) const;

  private:

    std::vector<T> data;

    /**
     * References to left and right children.
     * in same order as data (indexes are same).
     */
    std::vector<iterator> leftSuccessorChildren;
    std::vector<iterator> rightSuccessorChildren;
    std::vector<iterator> leftPredecessorChildren;
    std::vector<iterator> rightPredecessorChildren;
}; 

#include "../../../src/RangeTree/ArrayRT.cpp"

#endif
