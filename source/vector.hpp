#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <initializer_list>
#include <stdexcept>

#include "allocator.hpp"
#include "algorithm.hpp"
#include "iterator.hpp"
#include "memory.hpp"

__STLL_NAMESPACE_START__

template <class Tp, class Alloc=allocator<Tp>>
class vector {
public:
    typedef Tp               value_type;
    typedef Tp*              pointer;
    typedef const Tp*        const_pointer;
    typedef Tp&              reference;
    typedef const Tp&        const_reference;
    typedef size_t           size_type;
    typedef ptrdiff_t        difference_type;

    typedef pointer         iterator;
    typedef const_pointer   const_iterator;
    typedef Alloc           alloc;

    typedef vector<Tp, Alloc> self;
public:

    vector() {
        start = nullptr;
        finish = nullptr;
        end_of_storage = nullptr;
    }

    vector(size_type size) {
        start = alloc::allocate(size);
        finish = end_of_storage = start + size;
    }

    vector(size_type size, const value_type& value) {
        start = alloc::allocate(size);
        finish = end_of_storage = start + size;
        uninitialized_fill(start, finish, value);
    }

    vector(int size, const value_type& value)
        :vector(size_type(size), value)
    {}

    vector(unsigned int size, const value_type& value)
        :vector(size_type(size), value)
    {}


    vector(long size, const value_type& value)
        :vector(size_type(size), value)
    {}


    vector(long long size, const value_type& value)
        :vector(size_type(size), value)
    {}

     vector(unsigned long long size, const value_type& value)
        :vector(size_type(size), value)
    {}

    vector(const std::initializer_list<value_type>& value_list)
        :vector() {
        reserve(value_list.size());
        for (const value_type& value : value_list) {
            construct(finish, value);
            ++finish;
        }
    }

    template <class InputIterator>
    vector(InputIterator first, InputIterator last) {
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        construct_from_range(first, last, category());
    }


    vector(const self& vec) {
        size_type size = vec.size();
        start = alloc::allocate(size);
        finish = end_of_storage = start + size;
        copy(vec.begin(), vec.end(), start);
    }

    vector(self&& vec) {
        start = vec.start;
        finish = vec.finish;
        end_of_storage = vec.end_of_storage;
        vec.start = vec.finish = vec.end_of_storage = nullptr;
    }

    self& operator=(const self& vec) {
        if (vec.size() > capacity()) {
            extend_capacity(vec.size());
        }
        clear();
        copy(vec.begin(), vec.end(), start);
    }

    self& operator=(self&& vec) {
        start = vec.start;
        finish = vec.finish;
        end_of_storage = vec.end_of_storage;
    }

    ~vector() {
        if (start) {
            alloc::deallocate(start, capacity());
        }
        end_of_storage = finish = start = nullptr;
    }

    bool empty() const {
        return finish == start;
    }

    size_type size() const {
        return finish - start;
    }

    size_type capacity() const {
        return end_of_storage - start;
    }

    const value_type& at(size_type index) const {
        if (index >= size())
            throw std::range_error("index out of range");
        return *(start + index);
    }

    const value_type& operator[](size_type index) const {
        return *(start + index);
    }

    const_iterator begin() const {
        return const_iterator(start);
    }

    const_iterator end() const {
        return const_iterator(finish);
    }

    void reserve(size_type size) {
        extend_capacity(size);
    }

    void clear() {
        destroy(start, finish);
        finish = start;
    }

    iterator begin() {
        return start;
    }

    iterator end() {
        return  finish;
    }

    const_reference front() const {
        return *start;
    }

    const_reference back() const {
        return *(finish - 1);
    }

    value_type& operator[](size_type index) {
        return *(start + index);
    }

    value_type& at(size_type index) {
        if (index >= size())
            throw std::range_error("index out of range");
        return *(start + index);
    }

    void push_back(const value_type& value) {
        if (!(capacity() > size()))
            extend_capacity();
        *finish = value;
        ++finish;
    }

    template <class... Args>
    void emplace_back(Args... args) {
        if(!(capacity() > size()))
            extend_capacity();
        construct(finish, args...);
        ++finish;
    }

    void pop_back() {
        destroy(finish-1);
        --finish;
    }

    iterator insert(const iterator& pos, const value_type& value) {
        difference_type pos_index = pos - start;
        if (!(capacity() > size()))
            extend_capacity();

        iterator pos_iter = start + pos_index;
        move_backward(pos_iter, finish, finish+1);
        *pos_iter = value;
        ++finish;
        return ++pos_iter;
    }

    template <class InputIterator>
    iterator insert(const iterator& pos,
                    InputIterator first, InputIterator last) {
        difference_type length = distance(first, last);
        size_type pos_index = pos - start;
        if (size() + length > capacity())
            extend_capacity(max(capacity()*2, capacity()+length));

        iterator pos_iter = start + pos_index;
        std::move_backward(pos_iter, finish, finish+length);

        for (; first != last; ++first, ++pos_iter)
            *pos_iter = *first;
        return start + pos_index + length;
    }

    void shrink_to_fit() {
        if (size() == capacity())
            return;
        size_type old_size = size();
        iterator new_start = alloc::allocate(size());
        copy(begin(), end(), new_start);
        alloc::deallocate(start, capacity());
        start = new_start;
        end_of_storage = finish = start + old_size;
    }

protected:
    template <class InputIterator>
    void construct_from_range(InputIterator first, InputIterator last,
                              input_iterator_tag) {
        while (first != last) {
            push_back(*first);
        }
    }

    template <class RandomAcessIterator>
    void construct_from_range(RandomAcessIterator first,
                              RandomAcessIterator last,
                              random_access_iterator_tag) {
        typedef typename iterator_traits<RandomAcessIterator>::difference_type
                Distance;
        Distance size = last - first;
        start = alloc::allocate(size);
        end_of_storage = finish = start + size;
        copy(first, last, start);
    }

    // Exend vector's capacity to cap.
    // If capacity already greater than cap, do nothing.
    // If cap less than or equal to size(), do nothing.
    void extend_capacity(size_type cap=0) {
        if (cap == 0)
            cap = max(size_type(1), capacity() * 2);

        if (capacity() > cap || cap <= this->size()) return;

        size_type old_cap = capacity();
        iterator new_start = alloc::allocate(cap);
        copy(start, finish, new_start);
        start = new_start;
        finish = start + old_cap;
        end_of_storage = start + cap;
    }

    template <class InputIterator>
    void copy_from_range(InputIterator first, InputIterator last) {
        size_type range_size = distance(first, last);
        clear();

        if (capacity() < range_size)
            extend_capacity(range_size);
        finish = copy(first, last, start);
    }

protected:
    iterator start;
    iterator finish;
    iterator end_of_storage;
};

/*
template <class T, class Alloc>
inline bool operator<(const vector<T, Alloc>& vec1,
const vector<T, Alloc>& vec2) {
    return lexicographical_compare(vec1.begin(), vec1.end(),
                                   vec2.begin(), vec2.end());
}
*/


__STLL_NAMESPACE_FINISH__

#endif // VECTOR_HPP
