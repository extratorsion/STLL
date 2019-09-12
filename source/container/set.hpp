#ifndef SET_HPP
#define SET_HPP
#include "rbtree.hpp"

__STLL_NAMESPACE_START__

template <typename Key, typename Compare=less<Key>, typename Alloc=allocator<
        rb_tree_node<Key>
        >>

class set {
public:
    typedef Key         key_type;
    typedef Key         value_type;
    typedef Compare     key_compare;
    typedef Compare     value_compare;

protected:
    typedef rb_tree<key_type, value_type, identity<value_type>,
                    key_compare, Alloc> rep_type;
    typedef set<Key, Compare, Alloc>    self;
    rep_type            tree;

public:
    typedef typename rep_type::const_iterator  const_iterator;
    typedef typename rep_type::const_iterator  iterator;
    typedef typename rep_type::size_type       size_type;
    typedef typename rep_type::difference_type difference_type;
    typedef typename rep_type::const_pointer   const_pointer;
    typedef typename rep_type::const_pointer   pointer;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_reference reference;

public:
    set() = default;

    set(const Compare& comp)
           :tree(comp)
    {}

    set(const self&) = default;

    set(self&&) = default;

    template <typename InputIterator>
    set(InputIterator first, InputIterator last)
        :tree() {
        while (first != last) {
            insert(*first);
            ++first;
        }
    }

    set(const std::initializer_list<value_type>& value_list)
           :set(value_list.begin(), value_list.end())
    {}

    self& operator=(const self&) = default;

    self& operator=(self&&) = default;

    ~set() = default;

    key_compare key_comp() const {
        return tree.key_comp();
    }
    
    value_compare value_comp() const {
        return tree.key_comp();
    }

    iterator begin() const {
        return tree.begin();
    }

    iterator end() const {
        return tree.end();
    }

    bool empty() const {
        return tree.empty();
    }

    size_type size() const {
        return tree.size();
    }

    size_type max_size() const {
        return tree.max_size();
    }

    void swap(self& other) {
        tree.swap(other);
    }

    pair<iterator, bool> insert(const value_type& x) {
        pair<typename rep_type::iterator, bool> p = tree.insert(x);
        return pair<iterator, bool>{p.first, p.second};
    }

    iterator insert(iterator pos, const value_type& x) {
        using rep_iterator = typename rep_type::iterator;
        return tree.insert((rep_iterator&)pos, x);
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last) {
        while (first != last) {
            tree.insert(*first);
            ++first;
        }
    }

    size_type erase(const value_type& x) {
        return tree.erase(x);
    }

    iterator erase(const_iterator pos) {
        return tree.erase(pos);
    }

    template <typename InputIterator>
    void erase(InputIterator first, InputIterator last) {
        while (first != last) {
            tree.erase(*first);
            ++first;
        }
    }

    void clear() {
        tree.clear();
    }
    
    iterator find(const key_type& x) const {
        return tree.find(x);
    }

    iterator count(const key_type& x) const {
        return tree.count(x);
    }

};


__STLL_NAMESPACE_FINISH__
#endif // SET_HPP
