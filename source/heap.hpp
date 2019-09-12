#ifndef HEAP_HPP
#define HEAP_HPP

#include "type_traits.hpp"
#include "vector.hpp"
#include "functor.hpp"

__STLL_NAMESPACE_START__

namespace
{

template <typename RandomAcessIterator, typename Distance,
          typename Tp, typename Compare>
void __push_heap(RandomAcessIterator first, Distance top_index,
                Distance hole_index, Tp value, const Compare& comp) {
    if (hole_index == top_index) return;
    Distance parent_index = (hole_index - 1) / 2;
    while (hole_index > top_index && comp(*(first + parent_index), value)) {
        *(first + hole_index) = *(first + parent_index);
        hole_index = parent_index;
        parent_index = (hole_index - 1) / 2;
    }
    *(first + hole_index) = value;
}

}

template <typename RandomAcessIterator, typename Compare>
inline void push_heap(RandomAcessIterator first, RandomAcessIterator last,
                      const Compare& comp) {
    typedef typename iterator_traits<RandomAcessIterator>::difference_type
            Distance;

    __push_heap(first, Distance(0), Distance(last-first-1), *(last-1), comp);
}


template <typename RandomAcessIterator>
inline void push_heap(RandomAcessIterator first, RandomAcessIterator last) {
    typedef typename iterator_traits<RandomAcessIterator>::difference_type
            Distance;
    typedef typename iterator_traits<RandomAcessIterator>::value_type
            Tp;
    __push_heap(first, Distance(0), Distance(last-first-1),
               *(last-1), less<Tp>());
}


template <typename RandomAcessIterator, typename Compare>
void pop_heap(RandomAcessIterator first, RandomAcessIterator last,
                       const Compare& comp) {
    if (last - first <= 1) return;

    typedef typename type_identity<decltype(*last)>::raw_type Tp;
    typedef typename type_identity<decltype(last - first)>::raw_type Distance;

    Tp value = *(last - 1);
    *(last - 1) = *first;

    Distance len = ((last - first) - 1);

    // Precolate down.
    Distance hole_index = 0;
    Distance child_index = hole_index * 2 + 1;
    while (child_index < len) {
        if (child_index + 1 < len and
            comp(*(first + child_index), *(first + child_index+1))) {
            child_index += 1;
        }

        if (!comp(value, *(first + child_index)))
            break;

        *(first + hole_index) = *(first + child_index);
        hole_index = child_index;
        child_index = hole_index * 2 + 1;
    }
    *(first + hole_index) = value;
}

template <typename RandomAcessIterator>
inline void pop_heap(RandomAcessIterator first, RandomAcessIterator last) {
    typedef typename type_identity<decltype(*last)>::raw_type Tp;
    pop_heap(first, last, less<Tp>());
}

template <typename RandomAcessIterator, typename Compare>
void sort_heap(RandomAcessIterator first, RandomAcessIterator last,
               const Compare& comp) {
    while (last - first > 1) {
        pop_heap(first, last, comp);
        --last;
    }
}


template <typename RandomAcessIterator>
void sort_heap(RandomAcessIterator first, RandomAcessIterator last) {
    typedef typename iterator_traits<RandomAcessIterator>::value_type Tp;
    sort_heap(first, last, less<Tp>());
}

template <typename RandomAcessIterator, typename Compare>
void make_heap(RandomAcessIterator first, RandomAcessIterator last,
               const Compare& comp) {
    if (last - first < 2) return;
    typedef typename iterator_traits<RandomAcessIterator>::difference_type
            Distance;
    Distance length = 0;
    RandomAcessIterator iter = first;
    do {
        ++iter;
        push_heap(first, iter, comp);
    } while (iter != last);
}

template <typename RandomAcessIterator>
void make_heap(RandomAcessIterator first, RandomAcessIterator last) {
    typedef typename iterator_traits<RandomAcessIterator>::value_type Tp;
    make_heap(first, last, less<Tp>());
}



__STLL_NAMESPACE_FINISH__

#endif // HEAP_HPP
