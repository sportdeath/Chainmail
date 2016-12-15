#ifndef BINARYRT_HPP
#define BINARYRT_HPP

#include "sweater/RangeTree/RT.hpp"


template <typename T, typename Node, typename Derived>
class BinaryRT : public RT<T, Derived> {
  public:
    typedef typename std::vector<T>::iterator iterator;

    BinaryRT(){
      leftChild = nullptr;
      rightChild = nullptr;
      node = nullptr;
    };

    /**
     * Assigns max and min and sorts
     */
    BinaryRT (
        iterator begin, 
        iterator end,
        bool sorted);

    static iterator rightMin(iterator begin, iterator end);

    /**
     * Deletes children
     */
    ~BinaryRT();

    Node * getNode();

    Derived * getChild(bool left);

    const T & getMax() const;
    const T & getMin() const;

    bool chooseBranch(const T & e, bool left) const;

  protected:
    Derived * leftChild;
    Derived * rightChild;
    Node * node;
  
  private:
    T min;
    T max;
};

#include "../../../src/RangeTree/BinaryRT.cpp"

#endif
