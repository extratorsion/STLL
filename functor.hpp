#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include "base.hpp"
#include <algorithm>

__STLL_NAMESPACE_START__


template <typename Tp>
struct less {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 < elem2;;
    }
};

template <typename Tp>
struct greater {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 > elem2;;
    }
};

template <typename Tp>
struct equal {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 == elem2;;
    }
};

template <typename Tp>
struct less_equal {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 <= elem2;;
    }
};

template <typename Tp>
struct greater_equal {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 >= elem2;
    }
};

template  <typename Tp>
struct negate {
    Tp operator()(const Tp& elem) const {
        return -elem;
    }
};

__STLL_NAMESPACE_FINISH__


#endif // FUNCTOR_HPP
