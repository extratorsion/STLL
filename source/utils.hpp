#ifndef UTILS_HPP
#define UTILS_HPP

#include "base.hpp"

__STLL_NAMESPACE_START__

template <class Comparable>
const Comparable& max(const Comparable& elem1, const Comparable& elem2) {
    return elem1 > elem2 ? elem1 : elem2;
}

template <class Comparable>
const Comparable& min(const Comparable& elem1, const Comparable& elem2) {
    return elem1 < elem2 ? elem1 : elem2;
}

template <class Tp>
void swap(Tp& elem1, Tp& elem2) {
    Tp tmp = elem1;
    elem1 = elem2;
    elem2 = tmp;
}

__STLL_NAMESPACE_FINISH__

#endif // UTILS_HPP
