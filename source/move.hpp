#ifndef MOVE_HPP
#define MOVE_HPP

#include "type_traits.hpp"

__STLL_NAMESPACE_START__

template <typename Tp>
constexpr Tp&& forward(typename remove_reference<Tp>::type& val) {
    return static_cast<Tp&&>(val);
}

template <typename Tp>
constexpr Tp&& forward(typename remove_reference<Tp>::type&& val) {
    return static_cast<Tp&&>(val);
}

template <typename Tp>
constexpr typename remove_reference<Tp>::type&& move(Tp&& val) {
    return static_cast<typename remove_reference<Tp>::type&&>(val);
}



__STLL_NAMESPACE_FINISH__

#endif // MOVE_HPP