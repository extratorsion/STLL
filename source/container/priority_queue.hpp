#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include "allocator.hpp"
#include "vector.hpp"
#include "heap.hpp"

__STLL_NAMESPACE_START__

template <typename Tp, typename Sequence=vector<Tp>,
          typename Compare=less<typename Sequence::value_type>>
class priority_queue {
public:
    typedef typename Sequence::value_type       value_type;
    typedef typename Sequence::pointer          pointer;
    typedef typename Sequence::reference        reference;
    typedef typename Sequence::const_reference  const_reference;
    typedef typename Sequence::const_pointer    const_pointer;
    typedef typename Sequence::size_type        size_type;
    typedef typename Sequence::difference_type  difference_type;

    typedef priority_queue<Tp, Sequence, Compare> self;

protected:
    Sequence sequence;
    Compare  comp;

public:
    priority_queue() = default;

    explicit priority_queue(const Compare& comp)
        : sequence(), comp(comp)
    {}

    template <typename InputIterator>
    priority_queue(InputIterator first, InputIterator last,
                   const Compare& comp=
                   less<typename iterator_traits<InputIterator>::value_type>())
        :sequence(first, last), comp(comp)
    {
        make_heap(sequence.begin(), sequence.end(), comp);
    }

    priority_queue(const self&) = default;

    priority_queue(self&&) = default;

    ~priority_queue() = default;

    bool empty() const {
        return sequence.empty();
    }

    size_type size() const {
        return sequence.size();
    }

    const_reference top() const {
        return sequence.front();
    }

    void push(const value_type& value) {
        sequence.push_back(value);
        push_heap(sequence.begin(), sequence.end(), comp);
    }

    void pop() {
        pop_heap(sequence.begin(), sequence.end(), comp);
        sequence.pop_back();
    }

};

__STLL_NAMESPACE_FINISH__

#endif // PRIORITY_QUEUE_HPP
