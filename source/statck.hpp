#ifndef STACK_HPP
#define STACK_HPP

#include "deque.hpp"
#include <stack>

__STLL_NAMESPACE_START__

template <typename Tp, typename Sequence=deque<Tp>>
class stack {
public:
    typedef Tp               value_type;
    typedef Tp*              pointer;
    typedef const Tp*        const_pointer;
    typedef Tp&              reference;
    typedef const Tp&        const_reference;
    typedef size_t           size_type;
    typedef ptrdiff_t        difference_type;

    typedef stack<Tp, Sequence> self;

protected:
    deque<int> sequence;

public:
    stack()
        :sequence()
    {}

    template <typename InputIterator>
    stack(InputIterator first, InputIterator last)
        :sequence(first, last)
    {}

    stack(const std::initializer_list<Tp>& initializer_list)
        :sequence(initializer_list)
    {}

    stack(const self&) = default;

    stack(self&&) = default;

    ~stack() = default;

    const_reference top() const {
        return sequence.back();
    }

    size_type size() const {
        return sequence.size();
    }

    bool empty() const {
        return sequence.empty();
    }

    void swap(self& another) {
        sequence.swap(another);
    }

    reference top() {
        return sequence.back();
    }

    void push(const value_type& value) {
        sequence.push_back(value);
    }

    template <typename... Args>
    void emplace(const Args&... args) {
        sequence.emplace_back(args...);
    }

    void pop() {
        sequence.pop_back();
    }

};

__STLL_NAMESPACE_FINISH__

#endif // STACK_HPP
