#ifndef RT_HPP
#define RT_HPP

#include <vector>

template <typename T, typename Derived>
class RT {
  public:
    /**
     * Compares whether one element
     * is less than another
     */
    //virtual bool lessThan(const T & e1, const T & e2) = 0;

    /**
     * Get the maximum element in the tree.
     */
    virtual const T & getMax() const = 0;
    virtual const T & getMin() const = 0;

    /**
     * Whether or not e is a [left] bound
     *
     * left bound:
     * |----->
     *
     * right bound:
     * <-----|
     */
    bool isBound(const T & e, bool left) const;

    /**
     * Whether or not the data within
     * the RT intersects with the range e1...e2
     *
     * e1 < e2
     */
    bool intersects(const T & e1, const T & e2) const;

    /**
     * The largest iterator in a sorted array
     * that is to the current iterator.
     */
    static typename std::vector<T>::iterator getLargestEqualIndex(
        typename std::vector<T>::iterator it,
        typename std::vector<T>::iterator end
        );
    static typename std::vector<T>::iterator getSmallestEqualIndex(
        typename std::vector<T>::iterator it,
        typename std::vector<T>::iterator begin
        );
};

#include "../../../src/RangeTree/RT.cpp"

#endif
