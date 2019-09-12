#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "pair.hpp"
#include "type_traits.hpp"


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

template <typename Iterator1, typename Iterator2>
void iter_swap(Iterator1 iter1, Iterator2 iter2) {
    typedef typename iterator_traits<Iterator1>::value_type Tp;
    Tp tmp = *iter1;
    *iter1 = *iter2;
    *iter2 = tmp;
}

namespace
{

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(
           InputIterator first,
           InputIterator last,
           random_access_iterator_tag) {
    return last - first;
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(
           InputIterator first,
           InputIterator last,
           input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type distance_value = 0;

    while (first != last)
        ++first;

    return distance_value;
}

template <typename InputIterator, typename Distance>
Distance __advance(InputIterator& first, Distance n,
                   forward_iterator_tag) {
    while (n--) {
        ++first;
    }
}

template <typename InputIterator, typename Distance>
Distance __advance(InputIterator& first, Distance n,
                   random_access_iterator_tag) {
    first = first + n;
}

}


template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(
           InputIterator first,
           InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return __distance(first, last, category());
}

template <typename InputIterator, typename Distance>
inline typename iterator_traits<InputIterator>::difference_type advance(
           InputIterator& first,
           Distance n) {
    return __advance(first, n, iterator_category(first));
}

/* iterator adapt */
template <typename Container>
class back_insert_iterator {
protected:
    Container* container;
public:
    typedef output_iterator_tag     iterator_category;
    typedef void                    value_type;
    typedef void                    diffrence_type;
    typedef void                    pointer;
    typedef void                    reference;

    explicit back_insert_iterator(Container& x)
        :container(&x)
    {}

    back_insert_iterator<Container>&
    operator=(const typename Container::value_type& value) {
        container->push_back(value);
        return *this;
    }

    back_insert_iterator<Container>& operator*(){return *this;}

    back_insert_iterator<Container>& operator++(){return *this;}

    back_insert_iterator<Container>& operator++(int){return *this;}
};

template <typename Container>
inline back_insert_iterator<Container> back_inserter(Container& x) {
    return back_insert_iterator<Container>(x);
}

template <typename Container>
class front_insert_iterator {
protected:
    Container* container;
public:
    typedef output_iterator_tag     iterator_category;
    typedef void                    value_type;
    typedef void                    diffrence_type;
    typedef void                    pointer;
    typedef void                    reference;

    explicit front_insert_iterator(Container& x)
        :container(&x)
    {}

    front_insert_iterator<Container>&
    operator=(const typename Container::value_type& value) {
        container->push_front(value);
        return *this;
    }

    front_insert_iterator<Container>& operator*(){return *this;}

    front_insert_iterator<Container>& operator++(){return *this;}

    front_insert_iterator<Container>& operator++(int){return *this;}
};

template <typename Container>
inline front_insert_iterator<Container> front_inserter(Container& x) {
    return front_insert_iterator<Container>(x);
}

template <typename Container>
class insert_iterator {
protected:
    Container*  container;
    typename Container::iterator    iter;

public:
    typedef output_iterator_tag     iterator_category;
    typedef void                    value_type;
    typedef void                    diffrence_type;
    typedef void                    pointer;
    typedef void                    reference;

    typedef typename Container::iterator
                                    iterator;

    explicit insert_iterator(Container& x, iterator iter)
        :container(&x), iter(iter)
    {}

    insert_iterator<Container>&
    operator=(const typename Container::value_type& value) {
        iter = container->insert(iter, value);
        ++iter;
        return *this;
    }

    insert_iterator<Container>& operator*(){return *this;}

    insert_iterator<Container>& operator++() {return *this;}

    insert_iterator<Container> operator++(int){return *this;}
};

template <typename Container, typename Iterator>
inline insert_iterator<Container> inserter(Container& x, Iterator iter) {
    return insert_iterator<Container>(x, iter);
}


template <typename Iterator>
struct reverse_iterator {
    typedef typename iterator_traits<Iterator>::value_type   value_type;
    typedef typename iterator_traits<Iterator>::pointer      pointer;
    typedef typename iterator_traits<Iterator>::reference    reference;

    typedef typename iterator_traits<Iterator>::difference_type
                     differece_type;
    typedef typename iterator_traits<Iterator>::iterator_category
                     iterator_category;
    typedef Iterator iterator_type;
    typedef reverse_iterator<Iterator> self;

protected:
    Iterator iter;
public:
    reverse_iterator() = default;

    explicit reverse_iterator(const iterator_type& x)
        :iter(x)
    {}

    reverse_iterator(const self&) = default;

    iterator_type base() const {return  iter;}

    reference operator*() {
        Iterator tmp = iter;
        return *--tmp;
    }

    pointer operator->() const {
        return &(operator*());
    }

    self& operator++() {
         --iter;
        return *this;
    }

    self operator++(int) {
        Iterator tmp = iter;
         --iter;
        return tmp;
    }

    self operator--() {
        ++iter;
        return *this;
    }

    self operator--(int) {
        Iterator tmp = iter;
        ++iter;
        return tmp;
    }

    self& operator+=(differece_type step) {
        iter = iter - step;
        return *this;
    }

    self operator+(differece_type step) const {
        self ret_iter = *this;
        ret_iter.iter = ret_iter - step;
        return ret_iter;
    }

    self operator-(differece_type step) const {
        self ret_iter = *this;
        ret_iter.iter = ret_iter + step;
        return ret_iter;
    }

    value_type operator[](differece_type step) const {
        return *(this->operator+(step));
    }

    bool operator==(const reverse_iterator<Iterator>& another_iter) {
        return iter == another_iter.iter;
    }

    bool operator!=(const reverse_iterator<Iterator>& another_iter) {
        return iter != another_iter.iter;
    }
};


/* same as enumerate in python3
 * return {index, element}
 */
template <typename Container>
class  enumerate_t
{
public:
    using original_iterator = typename Container::iterator ;
    using self              =  enumerate_t<Container>;

    struct enumerate_iterator {
        using value_type = pair<size_t,
                    typename iterator_traits<original_iterator>::value_type>;
        using iteraotr_catefory = input_iterator_tag;
        using reference         = value_type&;
        using pointer           = value_type*;
        using difference_type   = ptrdiff_t;

        using self = enumerate_t<Container>::enumerate_iterator;

        original_iterator current;
        size_t            counter;

        enumerate_iterator(const original_iterator& iter, size_t counter)
            :current(iter)
            ,counter(counter)
        {}

        value_type operator*() {
            return {counter, *current};
        }

        self& operator++() {
            ++counter;
            ++current;
            return *this;
        }

        self operator++(int) {
            self tmp = *this;
            ++counter;
            ++current;
            return tmp;
        }

        bool operator==(const self& other) {
            return other.counter == counter and other.current == current;
        }

        bool operator!=(const self& other) {
            return !operator==(other);
        }
    };

protected:
    size_t              counter;
    original_iterator   obegin;
    original_iterator   oend;
    original_iterator   current;

protected:
    enumerate_t(size_t cnt, const original_iterator& obegin,
                const original_iterator& oend, const original_iterator& current)
        :obegin(obegin)
        ,oend(oend)
        ,current(current)
        ,counter(cnt)
    {}

public:
    enumerate_t(Container& containter, size_t start_cnt=0)
        :obegin(containter.begin())
        ,oend(containter.end())
        ,current(obegin)
        ,counter(start_cnt)
    {}


    enumerate_iterator begin() {
        return enumerate_iterator(obegin, counter);
    }

    enumerate_iterator end() {
        return enumerate_iterator(oend, counter + distance(obegin, oend));
    }
};

template <typename Container>
enumerate_t<Container>
enumerate(Container& container, size_t start_cnt=0) {
    return enumerate_t<Container>(container, start_cnt);
}

__STLL_NAMESPACE_FINISH__

#endif // ITERATOR_HPP
