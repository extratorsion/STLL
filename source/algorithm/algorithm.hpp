#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "algo_base.hpp"
#include "functor.hpp"
#include "numeric.hpp"
#include "memory.hpp"

__STLL_NAMESPACE_START__


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


template <typename InputIterator>
InputIterator adjacent_find(InputIterator first, InputIterator last) {
    InputIterator result = first;
    while (first != last) {
        ++first;
        if (*first == *result)
            return result;
        else
            ++result;
    }
    return result;
}

template <typename InputIterator, typename BinaryOperation>
InputIterator adjacent_find(InputIterator first, InputIterator last,
                            BinaryOperation binary_op) {
    InputIterator result = first;
    while (first != last) {
        ++first;
        if (binary_op(*first == *result))
            return result;
        else
            ++result;
    }
    return result;
}


template <typename ForwardIterator, typename Tp>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
                            const Tp& value) {
    typedef typename iterator_traits<ForwardIterator>::difference_type Distance;
    Distance len = distance(first, last);
    Distance half;
    ForwardIterator middle;
    while (len > 0) {
        half = (len >> 1);
        middle = advance(first, half);
        if (*middle < value) {
            first = middle;
            ++first;
            len = len - (half + 1);
        } else {
            len = half;
        }
    }
    return first;
}

template <typename BidrectionalIterator>
BidrectionalIterator reverse(BidrectionalIterator first,
                             BidrectionalIterator last) {
    typedef typename iterator_traits<BidrectionalIterator>::value_type Tp;
    while (first != last) {
        Tp tmp = *first;
        --last;
        *first = *last;
        *last = tmp;
        if (first != last)
            ++first;
    }
}

/* Permutation */
template <typename BidrectionalIterator>
bool next_permutaition(BidrectionalIterator first, BidrectionalIterator last) {
    if (first == last) return false;
    BidrectionalIterator iter1 = first;
    if (++iter1 == last) return false;

    iter1 = last;
    --iter1;
    BidrectionalIterator iter2 = iter1;
    --iter1;
    while (true) {
        if (*iter1 < *iter2)
            break;
        else {
            if (first == iter1)
                return false;
            --iter1;
            --iter2;
        }
    }

    BidrectionalIterator iter3 = last;
    --iter3;
    for (;;) {
        if (*iter3 > *iter1)
            break;
        --iter3;
    }
    iter_swap(iter1, iter3);
    reverse(iter2, last);
    return true;
}

template <typename BidrectionalIterator>
bool prev_permutaition(BidrectionalIterator first, BidrectionalIterator last) {
    if (first == last) return false;
    BidrectionalIterator iter1 = first;
    if (++iter1 == last) return false;

    iter1 = last;
    --iter1;
    BidrectionalIterator iter2 = iter1;
    --iter1;
    while (true) {
        if (*iter1 > *iter2)
            break;
        else {
            if (first == iter1)
                return false;
            --iter1;
            --iter2;
        }
    }

    BidrectionalIterator iter3 = last;
    --iter3;
    for (;;) {
        if (*iter3 < *iter1)
            break;
        --iter3;
    }
    iter_swap(iter1, iter3);
    reverse(iter2, last);
    return true;
}

/* Insertion Sort */
namespace
{

template <typename RandomAcessIterator>
void __unguarded_linear_insert(RandomAcessIterator last) {
    typedef typename iterator_traits<RandomAcessIterator>::value_type Tp;
    Tp value = *last;
    RandomAcessIterator prev = last;
    --prev;
    while (value < *prev) {
        *last = *prev;
        last = prev;
        --prev;
    }
    *last = value;
}

template <typename RandomAcessIterator>
void __linear_insert(RandomAcessIterator first, RandomAcessIterator last) {
    typedef typename iterator_traits<RandomAcessIterator>::value_type Tp;
    Tp value = *last;
    if (value < *first) {
        copy_backward(first, last, last + 1);
        *first = value;
    } else {
        __unguarded_linear_insert(last);
    }
}

}


template <typename RandomAcessIterator>
void insert_sort(RandomAcessIterator first, RandomAcessIterator last) {
    if (first == last) return;
    for (RandomAcessIterator iter = first + 1; iter != last; ++iter) {
        __linear_insert(first, iter);
    }
}


__STLL_NAMESPACE_FINISH__


#endif // ALGORITHM_HPP
