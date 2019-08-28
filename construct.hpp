#ifndef CONSTpRUCTp_HPP
#define CONSTpRUCTp_HPP

#include "type_traits.hpp"
#include "iterator.hpp"

__STLL_NAMESPACE_START__

namespace
{

// If has_trivial_destructor, then do nothing when destring object.
template <typename ForwardIterator>
inline void _destroy_aux(const ForwardIterator& /*first*/,
                         const ForwardIterator& /*last*/,
                         true_type)
{}


template <typename ForwardIterator>
inline void _destroy_aux(const ForwardIterator& first,
                         const ForwardIterator& last,
                         false_type)
{
    for (ForwardIterator iter = first; iter != last; ++iter)
        destroy(&*iter);
}

template <typename ForwardIterator, typename Tp>
inline void _destroy(const ForwardIterator& first,
                     const ForwardIterator& last,
                     Tp*)
{
    typedef typename type_traits<Tp>::has_trivial_destructor trivial_destructor;
    _destroy_aux(first, last, trivial_destructor());
}

}


/* Generic construct and destroy. */

// Construct object which ptr pointed with value.
template <typename Tp1, typename Tp2>
inline void construct(Tp1* ptr, const Tp2& value) {
    new(ptr)Tp2(value); // placement new
}

// Construct object which ptr pointed with args.
template <typename Tp, typename ...Args>
inline void construct(Tp* ptr, Args&&... args) {
    new(ptr)Tp(args...);
}

// Destroy object which ptr pointed.
template <typename Tp>
inline void destroy(Tp* ptr) {
    ptr->~Tp();
}

template <typename ForwardIterator>
inline void destroy(const ForwardIterator& first, const ForwardIterator& last) {
    _destroy(first, last, value_type(first));
}

__STLL_NAMESPACE_FINISH__

#endif // CONSTRUCT_HPP
