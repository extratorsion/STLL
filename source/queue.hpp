#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "deque.hpp"

__STLL_NAMESPACE_START__

template <typename Tp, typename Sequence=deque<Tp>>
class queue {
public:
    typedef Tp               value_type;
    typedef Tp*              pointer;
    typedef const Tp*        const_pointer;
    typedef Tp&              reference;
    typedef const Tp&        const_reference;
    typedef size_t           size_type;
    typedef ptrdiff_t        difference_type;

    typedef queue<Tp, Sequence> self;

protected:
    Sequence sequence;

public:
    queue()
        :sequence()
    {}

    template <typename InputIterator>
    queue(InputIterator first, InputIterator last)
        :sequence(first, last)
    {}

    queue(const std::initializer_list<Tp>& initializer_list)
        :sequence(initializer_list)
    {}

    queue(const self&) = default;

    queue(self&&) = default;

    ~queue() = default;

    const_reference front() const {
        return sequence.front();
    }

    size_type size() const noexcept {
        return sequence.size();
    }

    bool empty() const {
        return sequence.empty();
    }

    reference front() {
        return sequence.front();
    }

    void pop() {
        sequence.pop_front();
    }

    void push(const value_type& value) {
        sequence.push_back(value);
    }

    template <typename... Args>
    void emplace(const Args... args) {
        sequence.emplace_back(args...);
    }

    void swap(self& another) {
        sequence.swap(another);
    }

};

__STLL_NAMESPACE_FINISH__

#endif // QUEUE_HPP
