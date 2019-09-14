#ifndef MAP_HPP
#define MAP_HPP
#include "rbtree.hpp"

__STLL_NAMESPACE_START__

template <typename Key, class Tp, typename Compare=less<Key>,
          typename Alloc=allocator<rb_tree_node<pair<Key, Tp>>>>
class map {
public:
    typedef Key             key_type;
    typedef pair<Key, Tp>   value_type;
    typedef Compare         key_compare;
    typedef Tp              data_type;
    typedef Tp              mapped_type;

    class value_compare
            :public binary_function<value_type, value_type, bool> {
        friend  class map<Key, Tp, Compare, Alloc>;
    protected:
        Compare comp;
        value_compare(Compare c)
            :comp(c)
        {}

    public:
        bool operator()(const value_type& x, const value_type& y) const {
            return comp(x.first, y.first);
        }
    };


protected:
    typedef rb_tree<key_type, value_type, identity<value_type>,
                    value_compare, Alloc>     rep_type;
    typedef map<Key, Tp, Compare, Alloc>      self;
    rep_type                                  tree;

public:
    typedef typename rep_type::const_iterator  const_iterator;
    typedef typename rep_type::iterator        iterator;
    typedef typename rep_type::size_type       size_type;
    typedef typename rep_type::difference_type difference_type;
    typedef typename rep_type::const_pointer   const_pointer;
    typedef typename rep_type::const_pointer   pointer;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_reference reference;

public:
    map()
        :tree(value_compare(key_compare()))
    {}

    map(const Compare& comp)
        :tree(value_comppare(comp))
    {}

    map(const self&) = default;

    map(self&&) = default;

    template <typename InputIterator>
    map(InputIterator first, InputIterator last)
        :map() {
        while (first != last) {
            insert(*first);
            ++first;
        }
    }

    map(const std::initializer_list<value_type>& value_list)
           :map(value_list.begin(), value_list.end())
    {}

    self& operator=(const self&) = default;

    self& operator=(self&&) = default;

    ~map() = default;

    key_compare key_comp() const {
        return tree.key_comp();
    }

    value_compare value_comp() const {
        return value_compare(tree.key_comp());
    }

    const_iterator begin() const {
        return tree.begin();
    }

    const_iterator end() const {
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

    const_iterator find(const key_type& x) const {
        return tree.find(x);
    }

    const_iterator count(const key_type& x) const {
        return tree.count(x);
    }

    void swap(self& other) {
        tree.swap(other);
    }

    Tp& operator[](const key_type& key) {
        return (*(insert({key, Tp()})).first).second;
    }

    pair<iterator, bool> insert(const value_type& x) {
        return tree.insert(x);
    }

    iterator insert(iterator pos, const value_type& x) {
        return tree.insert(pos, x);
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

    iterator begin() {
        return tree.begin();
    }

    iterator end() {
        return tree.end();
    }

    iterator find(const key_type& x) {
        return tree.find(x);
    }

};

__STLL_NAMESPACE_FINISH__

#endif // MAP_HPP
