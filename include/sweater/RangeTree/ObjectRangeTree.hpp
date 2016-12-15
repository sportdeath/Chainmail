#ifndef OBJECT_RANGE_TREE_HPP
#define OBJECT_RANGE_TREE_HPP

#include "sweater/RangeTree/BinaryRT1.hpp"
#include "sweater/RangeTree/BinaryRT0.hpp"
#include "sweater/RangeTree/ArrayRT.hpp"
#include "sweater/Objects/Object.hpp"

class ObjectRangeTreeZ :
  public ArrayRT<Object *, ObjectRangeTreeZ> {
  public:
    using ArrayRT<Object *, ObjectRangeTreeZ>::ArrayRT;

    static bool lessThan(const Object * e1, const Object * e2);
    static bool equals(const Object * e1, const Object * e2);
};

class ObjectRangeTreeY :
  public BinaryRT0<Object *, ObjectRangeTreeZ, ObjectRangeTreeY> {
  public:
    using BinaryRT0<Object *, ObjectRangeTreeZ, ObjectRangeTreeY>::BinaryRT0;

    static bool lessThan(const Object * e1, const Object * e2);
    static bool equals(const Object * e1, const Object * e2);
};


class ObjectRangeTreeX : 
  public BinaryRT1<Object *, ObjectRangeTreeY, ObjectRangeTreeX> {
  public:
    using BinaryRT1<Object *, ObjectRangeTreeY, ObjectRangeTreeX>::BinaryRT1;

    static bool lessThan(const Object * e1, const Object * e2);
    static bool equals(const Object * e1, const Object * e2);

    std::vector<Object*>::iterator begin() {
      return getNode() -> getNode() -> begin();
    }
    std::vector<Object*>::iterator end() {
      return getNode() -> getNode() -> end();
    }
};


#endif
