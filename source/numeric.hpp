#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include "type_traits.hpp"
#include "iterator.hpp"
#include "functor.hpp"

__STLL_NAMESPACE_START__

template <typename InputIterator, typename Tp>
Tp accumulate(InputIterator first, InputIterator last, Tp init) {
    while (first != last) {
        init = (init + *(first));
    }
    return init;
}

template <typename InputIterator, typename Tp, typename BinaryOperation>
Tp accumulate(InputIterator first, InputIterator last, Tp init,
              BinaryOperation binary_operation) {
    while (first != last) {
        init = binary_operation(init, *(first));
    }
    return init;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result) {
    typedef typename iterator_traits<InputIterator>::value_type Tp;
    if (first == last)
        return;
    *result = *first;
    Tp prev_value = *first;
    while (++first != last) {
        *(++result) = *(first) - prev_value;
        prev_value = *first;
    }
    return result;
}

template <typename InputIterator, typename OutputIterator,
          typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result, 
                                   BinaryOperation binary_operation) {
    if (first == last) return;
    *result = *first;
    typedef typename iterator_traits<InputIterator>::value_type Tp; 
    Tp prev_value = *first;
    while (++first != last) {
        *(++result) = binary_operation((first) - prev_value);
        prev_value = *first;
    }
    return result;
}

template <typename InputIterator1, typename InputIterator2, typename Tp>
Tp inner_product(InputIterator1 first1, InputIterator1 last1,
                 InputIterator2 first2, Tp init) {
    while (first1 != last1) {
        init = init + (*first1 * *first2);
        ++first1;
        ++first2;
    }
    return init;
}

template <typename InputIterator1, typename InputIterator2,
          typename Tp, typename BinaryOperation>
Tp inner_product(InputIterator1 first1, InputIterator1 last1,
                 InputIterator2 first2, Tp init,
                 BinaryOperation binary_operation) {
    while (first1 != last1) {
        init = init + binary_operation(*first1, *first2);
        ++first1;
        ++first2;
    }
    return init;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result) {
    typedef typename iterator_traits<InputIterator>::value_type Tp;
    if (first == last) return;

    Tp value = *first; 
    *result = value;
    while (++first != last) {
        value = value + *first;
        *++result = value;
    }
    return ++result;
}

template <typename InputIterator, typename OutputIterator,
          typename BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result, 
                           BinaryOperation binary_operation
                           ) {
    typedef typename iterator_traits<InputIterator>::value_type Tp;
    if (first == last) return;

    Tp value = *first; 
    *result = value;
    while (++first != last) {
        value = binary_opeartion(value + *first);
        *++result = value;
    }
    return ++result;
}

template <typename Tp, typename Integer, typename MonoidOperation>
Tp power(Tp x, Integer n, MonoidOperation op) {
    if (n == 0)
        return 1;
    else {
        while ((n & 1) == 0) {
            n >>= 1;
            x = op(x, x);
        }
        Tp result = x;
        n >>= 1;
        while (n != 0) {
            x = op(x, x);
            if ((n & 1) != 0) 
                result = op(result, x);
            n >>= 1;
        }
        return result;
    }
}

template <typename Tp, typename Integer>
Tp power(Tp x, Integer n) {
    return power(x, n, multiplies<Tp>());
}


__STLL_NAMESPACE_FINISH__

#endif
