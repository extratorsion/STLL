#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "iterator.hpp"
#include "type_traits.hpp"
#include "functor.hpp"
#include <algorithm>
#include <functional>

__STLL_NAMESPACE_START__

namespace
{
template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type _distance(
           InputIterator first,
           InputIterator last,
           random_access_iterator_tag) {
    return last - first;
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type _distance(
           InputIterator first,
           InputIterator last,
           input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type distance_value = 0;

    while (first != last)
        ++first;

    return distance_value;
}


}


template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(
           InputIterator first,
           InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return _distance(first, last, category());
}


template <typename InputIterator1, typename InputIterator2, typename Pred>
bool lexicographical_compare(
        InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2,
        const Pred& pred) {

    while (first1 != last1 && first2 != last2) {
        if (pred(*first1, *first2))
            return true;
        else if (*first1 != *first2)
            return false;
    }
    return first1 == last1;
}

template <typename InputIterator1, typename InputIterator2, typename Pred>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2) {
    typedef typename iterator_traits<InputIterator1>::value_type value_type;

    return lexicographical_compare(first1, last1,
                                   first2, last2,
                                   less<value_type>());
}


__STLL_NAMESPACE_FINISH__


#endif // ALGORITHM_HPP
