#ifndef MEMORY_H
#define MEMORY_H


#include <cstring>
#include "construct.hpp"
#include "move.hpp"

__STLL_NAMESPACE_START__


namespace
{
/* uninitialized_copy */
template <typename InputIterator, typename ForwardIterator, typename Tp>
inline ForwardIterator __uninitialized_copy(InputIterator first,
                                            InputIterator last,
                                            ForwardIterator result, Tp*) ;
template <typename InputIterator, typename  ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
                                                InputIterator last,
                                                ForwardIterator result,
                                                true_type);

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
                                               InputIterator last,
                                               ForwardIterator result,
                                               false_type);

/* uninitialized_fill */
template <typename InputIterator, typename Tp>
inline InputIterator __uninitialized_fill(InputIterator first,
                                          InputIterator last,
                                          const Tp& value, Tp*);

template <typename InputIterator, typename Tp>
inline InputIterator __uninitialized_fill_aux(InputIterator first,
                                              InputIterator last,
                                              const Tp& value, true_type);

template <typename InputIterator, typename Tp>
inline InputIterator __uninitialized_fill_aux(InputIterator first,
                                              InputIterator last,
                                              const Tp& value, false_type);

/* uninitialized_fill_n */
template <typename ForwardIterator, typename Distance, typename Tp>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,
                                              Distance n,
                                              const Tp& value);

template <typename ForwardIterator, typename Distance, typename Tp>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
                                                  ForwardIterator last,
                                                  const Tp& value, true_type);

template <typename ForwardIterator, typename Distance, typename Tp>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
                                                  ForwardIterator last,
                                                  const Tp& value, false_type);

}

/*
 Copy the range [first, last) into [result, result + (last-first)).
*/
template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first,
                                   InputIterator last,
                                   ForwardIterator result) {
    return __uninitialized_copy(first, last, result, value_type(result));
}

template <char*, char*>
char* uninitialized_copy(const char* first, const char* last, char* result) {
    std::memmove(result, first, size_t(last - first));
    return result + (last - first);
}

template <wchar_t*, wchar_t*>
wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last,
                            wchar_t* result) {
    std::memmove(result, first, size_t(last - first));
    return result + (last - first);
}


template <typename InputIterator, typename Tp>
InputIterator uninitialized_fill(InputIterator first, InputIterator last,
                                 const Tp& value) {
    return __uninitialized_fill(first, last, value, value_type(first));
}

template <typename ForwardIterator, typename Distance, typename Tp>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Distance n,
                                     const Tp& value) {
    return __uninitialized_fill_n(first, n, value, value_type(first));
}


template <typename InputIterator, typename ForwardIterator>
ForwardIterator copy(InputIterator first, InputIterator last,
                     ForwardIterator result) {
    while (first != last) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator copy_backward(InputIterator first, InputIterator last,
                              ForwardIterator result) {
    while (last != first) {
        *(--result) = *(--last);
    }
    return result;
}


template <typename BidrectionalIterator1, typename BidrectionalIterator2>
BidrectionalIterator2 move(BidrectionalIterator1 first,
                           BidrectionalIterator1 last,
                           BidrectionalIterator2 result) {
    while (first != last) {
        *result = move(*first);
        ++first;
        ++result;
    }
    return result;
}

template <typename BidrectionalIterator1, typename BidrectionalIterator2>
BidrectionalIterator2 move_backward(BidrectionalIterator1 first,
                                    BidrectionalIterator1 last,
                                    BidrectionalIterator2 result) {
   while (last != first) {
        *(--result) = move(*(--last));
    }
    return result;
}


namespace
{

/* uninitialized_copy */
template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
                                               InputIterator last,
                                               ForwardIterator result,
                                               true_type) {
    return copy(first, last, result);
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
                                               InputIterator last,
                                               ForwardIterator result,
                                               false_type) {
    while (first != last) {
        construct(&*result, *first);
        ++first;
        ++result;
    }
    return result;
}


template <typename InputIterator, typename ForwardIterator, typename Tp>
inline ForwardIterator __uninitialized_copy(InputIterator first,
                                            InputIterator last,
                                            ForwardIterator result, Tp*) {
    typedef typename type_traits<Tp>::is_POD_type is_POD;
    return __uninitialized_copy_aux(first, last, result, is_POD());
}

/* uninitialized_fill */
template <typename InputIterator, typename Tp>
inline InputIterator __uninitialized_fill(InputIterator first,
                                          InputIterator last,
                                          const Tp& value, Tp*) {
    typedef typename type_traits<Tp>::is_POD_type is_POD;
    return __uninitialized_fill_aux(first, last, value, is_POD());
}

template <typename InputIterator, typename Tp>
inline InputIterator __uninitialized_fill_aux(InputIterator first,
                                              InputIterator last,
                                              const Tp& value, true_type) {
    while (first != last) {
        *first = value;
        ++first;
    }
}

template <typename InputIterator, typename Tp>
inline InputIterator __uninitialized_fill_aux(InputIterator first,
                                              InputIterator last,
                                              const Tp& value, false_type) {
    while (first != last) {
        construct(&*first, value);
        ++first;
    }
}

/* uninitialized_fill_n */
template <typename ForwardIterator, typename Distance,
          typename Tp, typename Tp1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,
                                              Distance n,
                                              const Tp& value, Tp1*) {
    typedef typename type_traits<Tp1>::is_POD_type  is_POD;
    return __uninitialized_fill_n_aux(first, n, value, is_POD());
}

template <typename ForwardIterator, typename Distance, typename Tp>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
                                                  Distance n,
                                                  const Tp& value, true_type) {
    Distance i = 0;
    while (i++ < n) {
        *first = value;
        ++first;
    }
    return first;
}

template <typename ForwardIterator, typename Distance, typename Tp>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
                                                  Distance n,
                                                  const Tp& value, false_type) {
    Distance i = 0;
    while (i++ < n) {
        construct(&*first, value);
        ++first;
    }
    return first;

}

}
__STLL_NAMESPACE_FINISH__

#endif // MEMORY_H
