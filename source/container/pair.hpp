#ifndef PAIR_HPP
#define PAIR_HPP

#include "base.hpp"

__STLL_NAMESPACE_START__

template <typename Tp1, typename Tp2>
struct pair {
    typedef Tp1     first_type;
    typedef Tp2     second_type;
    first_type first;
    second_type second;
};

template <typename first_type, typename second_type>
pair<first_type, second_type> make_pair(const first_type& first,
                                        const second_type& second) {
    return pair<first_type, second_type>{first, second};
}

__STLL_NAMESPACE_FINISH__

#endif // PAIR_HPP
