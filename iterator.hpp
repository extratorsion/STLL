#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "base.hpp"

__STLL_NAMESPACE_START__


/*
 * input_iterator_tag:          readable
 * output_iterator_tag:         writable
 * forward_iterator_tag:        writable and increasable
 * bidirectional_iterator_tag:  writable, forward increasable and backward
 *                              increasable
 * random_access_iterator_tag:  writable, can random acess
 */

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag: public input_iterator_tag{};
struct bidirectional_iterator_tag:  public forward_iterator_tag {};
struct random_access_iterator_tag: public bidirectional_iterator_tag{};


template <typename IteratorCategory, typename Tp,
          typename Distance=size_t,
          typename Pointer=Tp*,
          typename Reference=Tp&>
struct iterator {
    typedef IteratorCategory        iterator_category;
    typedef Tp                      value_type;
    typedef Pointer                 pointer;
    typedef Reference               reference;
    typedef Distance                difference_type;
};



/* Iterator type traits. */
template <typename Iter>
struct iterator_traits {
    typedef typename Iter::iterator_category   iterator_category;
    typedef typename Iter::value_type          value_type;
    typedef typename Iter::pointer             pointer;
    typedef typename Iter::reference           reference;
    typedef typename Iter::difference_type     difference_type;
};

template<typename Tp>
struct iterator_traits<Tp*> {
    typedef random_access_iterator_tag      iterator_category;
    typedef Tp                              value_type;
    typedef Tp*                             pointer;
    typedef Tp&                             reference;
    typedef ptrdiff_t                       difference_type;
};

template <typename Tp>
struct iterator_traits<const Tp*> {
    typedef random_access_iterator_tag      iterator_category;
    typedef Tp                              value_type;
    typedef const Tp*                       pointer;
    typedef const Tp&                       reference;
    typedef ptrdiff_t                       difference_type;
};


template <typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&) {
    return typename iterator_traits<Iterator>::iterator_category();
}

// Return value_type* not value_type, for avoid consume of construct the object.
template <typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}


template <typename Iterator>
struct reverse_iterator {
    typedef typename iterator_traits<Iterator>::pointer      pointer;
    typedef typename iterator_traits<Iterator>::reference    reference;
    typedef typename iterator_traits<Iterator>::size_type    size_type;

    typedef typename iterator_traits<Iterator>::difference_type
                     differece_type;
    typedef typename iterator_traits<Iterator>::iterator_category
                     iterator_category;

protected:
    Iterator iter;
public:

    Iterator operator++() {
        return --iter;
    }

    Iterator operator++(int) {
        Iterator tmp = iter;
         --iter;
        return tmp;
    }


    Iterator operator--() {
        return ++iter;
    }

    Iterator operator--(int) {
         Iterator tmp = iter;
         ++iter;
        return tmp;
    }

    bool operator==(const reverse_iterator<Iterator>& another_iter) {
        return iter == another_iter;
    }

    bool operator!=(const reverse_iterator<Iterator>& another_iter) {
        return iter != another_iter;
    }

};


__STLL_NAMESPACE_FINISH__

#endif // ITERATOR_HPP
