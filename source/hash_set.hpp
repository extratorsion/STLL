#ifndef HASH_SET_HPP
#define HASH_SET_HPP

#include "hash_table.hpp"

__STLL_NAMESPACE_START__

template <typename Value,
          typename HashFun=hash<Value>,
          typename EqualKey=equal_to<Value>,
          typename Alloc=allocator<hashtable_node<Value>>>
class hash_set {
protected:
    typedef hash_table<Value, Value, HashFun, identity<Value>, EqualKey, Alloc>
                                                        table_type;
    typedef hash_set<Value, HashFun, EqualKey, Alloc>   self;
    table_type   table;

public:
    typedef EqualKey                               key_equal;

    typedef typename table_type::key_type          key_type;
    typedef typename table_type::value_type        value_type;
    typedef typename table_type::hasher            hasher;

    typedef typename table_type::size_type         size_type;
    typedef typename table_type::difference_type   difference_type;
    typedef typename table_type::const_pointer     pointer;
    typedef typename table_type::const_pointer     const_pointer;
    typedef typename table_type::const_reference   reference;
    typedef typename table_type::const_reference   const_reference;

    typedef typename table_type::const_iterator    iterator;
    typedef typename table_type::const_iterator    const_iterator;

public:
    hash_set()
        :table(size_t(100), hasher(), key_equal())
    {}

    explicit hash_set(size_type n)
        :table(n, hasher(), key_equal())
    {}

    hash_set(size_type n, const hasher& hash_fn)
        :table(n, hash_fn, key_equal())
    {}

    hash_set(size_type n, const hasher& hash_fn, const key_equal& key_eq)
        :table(n, hash_fn, key_eq)
    {}

    template <typename InputIterator>
    hash_set(InputIterator first, InputIterator last)
        :hash_set() {
            while (first != last) {
                table.insert_unique(*first);
                ++first;
            }
    }

    template <typename InputIterator>
    hash_set(InputIterator first, InputIterator last, size_type n,
             const hasher& hash_fn)
        :hash_set(n, hash_fn, key_equal()) {
            while (first != last) {
                table.insert_unique(*first);
                ++first;
            }
    }

    hash_set(const self&) = default;

    hash_set(self&&) = default;

    self& operator=(const self&) = default;

    self& operator=(self&&) = default;

    size_type size() const {
        return table.size();
    }

    size_type max_size() const {
        return table.max_size();
    }

    bool empty() const {
        return table.empty();
    }

    iterator begin() {
        return table.cbeing();
    }

    iterator end() {
        return table.cend();
    }

    const_iterator cbegin() const {
        return table.cbeing();
    }

    const_iterator cend() const {
        return table.cend();
    }

    void swap(self& another)  {
        return table.swap(another);
    }

    hasher hash_function() const {
        return table.hash_function();
    }

    key_equal key_eq() const {
        return table.key_eq();
    } 

    pair<iterator, bool> insert(const value_type& obj) {
        pair<typename table_type::iterator, bool> res =
                               table.insert_unique(obj);
         iterator iter = res.first;
         return make_pair(iter, res.second);
    }

    iterator find(const key_type& key) const {
        return table.find(key);
    }

    iterator count(const key_type& key) const {
        return table.count(key);
    }

    size_type erase(const key_type& key) {
        return table.erase(key);
    }

    void erase(iterator pos) {
        table.erase(pos);
    }

    void clear() {
        table.clear();
    }

    void resize(size_type size_hint) {
        table.resize(size_hint);
    }

    size_type bucket_count() const {
        return table.bucket_count();
    }

};


__STLL_NAMESPACE_FINISH__


#endif // HASH_SET_HPP
