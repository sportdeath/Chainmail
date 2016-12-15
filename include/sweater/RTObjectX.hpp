#include "sweater/RangeTree/BinaryRT0.hpp"
#include "sweater/RangeTree/BinaryRT1.hpp"

class RTObjectX : BinaryRT1<Object *, RTY> {
  bool lessThan(const & Object * e1, const & Object * e2);
}
