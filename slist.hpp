#ifndef SLIST_HPP
#define SLIST_HPP

#include "allocator.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include <forward_list>

__STLL_NAMESPACE_START__

namespace
{

template <typename Tp>
struct slist_node {
    slist_node<Tp>* next;
    Tp data;
};


template <typename Tp, typename Ref, typename Ptr>
struct slist_iterator {
    typedef forward_iterator_tag    iterator_category;
    typedef Tp                       value_type;
    typedef Ref                     reference;
    typedef Ptr                     pointer;
    typedef ptrdiff_t               difference_type;
    typedef size_t                  size_type;

    typedef slist_node<Tp>           node_type;

    typedef slist_iterator<Tp, Ref, Ptr>             self;
    typedef slist_iterator<Tp, Tp&, Tp*>               iterator;
    typedef slist_iterator<Tp, const Tp&, const Tp*>   const_iterator;

    node_type* node;

    slist_iterator(node_type* node)
        :node(node)
    {}

    self& operator++() {
        node = node->next;
        return *this;
    }

    self operator++(int) {
        node_type* ret = node;
        node = node->next;
        return ret;
    }

    reference operator*() {
        return node->data;
    }

    pointer operator->() {
        return &(operator*());
    }

};

}

template <typename Tp, typename Alloc=allocator<slist_node<Tp>>>
class slist {
public:
    typedef Tp           value_type;
    typedef Tp&          reference;
    typedef const Tp&    const_reference;
    typedef Tp*          pointer;
    typedef const Tp*    const_pointer;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

    typedef Alloc        node_alloc;

    typedef slist<Tp, Alloc>               self;

    typedef slist_iterator<Tp, Tp&, Tp*>   iterator;
    typedef slist_iterator<Tp, const Tp&, const Tp*> const_iterator;

protected:
    slist_node<Tp>       head;
    typedef slist_node<Tp> node_type;

public:
    slist() {
        head.next = nullptr;
    }

    slist(self&& L) {
        head.next = L.head.next;
        L.head.next = nullptr;
    }

    ~slist() {
        clear();
    }

    const_iterator begin() const {
        return const_iterator(head.next);
    }

    const_iterator end() const {
        return const_iterator(nullptr);
    }

    size_type size() const {
        size_type length = 0;
        node_type* p_node = head.next;
        while (p_node) {
            ++length;
            p_node = p_node->next;
        }
        return length;
    }

    iterator insert(const iterator& iter, const value_type& value) {
        node_type* new_node = create_node(value);
        new_node->next = iter.node->next;
        iter.node->next = new_node;
        return iterator(new_node);
    }

    bool empty() const {
        return head.next == nullptr;
    }

    void clear() {
        node_type* p_node = head.next;
        node_type* p_tmp = nullptr;
        while (p_node) {
            p_tmp = p_node->next;
            node_alloc::deallocate(p_node);
            p_node = p_tmp;
        }
    }

    iterator begin() {
        return iterator(head.next);
    }

    iterator end() {
        return iterator(nullptr);
    }

    reference front() {
        return (head.next)->data;
    }

    void push_front(const value_type& value) {
        node_type* new_node = create_node(value);
        new_node->next = head.next;
        head.next = new_node;
    }

    void pop_font() {
        node_type* p_pop = head.next;
        head.next = p_pop->next;
        destroy_node(p_pop);
    }

    void swap(self& L) {
        swap(L.head.next, head.next);
    }

    static node_type* create_node(const value_type& value) {
        node_type* new_node = node_alloc::allocate(1);
        construct(&new_node->data, value);
        new_node->next = nullptr;
    }

    static void destroy_node(node_type* node) {
        node_alloc::deallocate(node, 1);
    }

};


__STLL_NAMESPACE_FINISH__

#endif // SLIST_HPP
