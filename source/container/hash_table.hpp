#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "allocator.hpp"
#include "iterator.hpp"
#include "functor.hpp"
#include "vector.hpp"
#include "memory.hpp"
#include "hash.hpp"


__STLL_NAMESPACE_START__


template <class Value>
struct hashtable_node {
    hashtable_node* next;
    Value           data;    
};

template <typename Value, typename Key, typename HashFun, typename ExtractKey,
          typename EqualKey, typename Alloc>
class hash_table;

template <typename Value, typename Key, typename HashFun, typename ExtractKey,
          typename EqualKey, typename Alloc>
struct hashtable_iterator;

template <typename Value, typename Key, typename HashFun, typename ExtractKey,
          typename EqualKey, typename Alloc = allocator<hashtable_node<Value>>
          >
struct hashtable_const_iterator {
    typedef hash_table<Value, Key, HashFun, ExtractKey, EqualKey, Alloc>
    hashtable_type;
    typedef hashtable_const_iterator<Value, Key, HashFun, ExtractKey, EqualKey,
    Alloc>
    const_iterator;

    typedef const_iterator          iterator;

    typedef forward_iterator_tag    iterator_category;
    typedef Value                   value_type;
    typedef ptrdiff_t               difference;
    typedef size_t                  size_type;
    typedef const Value*            pointer;
    typedef const Value&            reference;
    typedef const Value&            const_reference;
    typedef const Value*            const_pointer;

    typedef hashtable_node<Value>   node_type;

public:
    const node_type*          cur;
    const hashtable_type*     table;

    hashtable_const_iterator() = default;

    hashtable_const_iterator(const node_type* node, const hashtable_type* table)
        : cur(node)
        , table(table)
    {}

    bool operator==(const const_iterator& another) const {
        return cur == another.cur and table == another.table;
    }

    bool operator!=(const const_iterator& another) const {
        return !operator==(another);
    }

    const_reference operator*() const {
        return cur->data;
    }

    const_pointer operator->() const {
        return &(operator*());
    }

    reference operator*() {
        return cur->data;
    }

    pointer operator->() {
        return &(operator*());
    }

    iterator& operator++() {
        const node_type* origin = cur;
        cur = cur->next;
        if (nullptr == cur) {
            size_type bucket = table->bucket_number(origin->data);
            while (cur == nullptr and ++bucket < table->buckets.size())
                cur = table->buckets[bucket];
        }
        return *this;
    }

    iterator operator++(int) {
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }

};

template <typename Value, typename Key, typename HashFun, typename ExtractKey,
          typename EqualKey, typename Alloc = allocator<hashtable_node<Value>>
          >

struct hashtable_iterator {
    typedef hash_table<Value, Key, HashFun, ExtractKey, EqualKey, Alloc>
    hashtable_type;
    typedef hashtable_iterator<Value, Key, HashFun, ExtractKey, EqualKey,
    Alloc>
    iterator;

    typedef hashtable_const_iterator<Value, Key, HashFun, ExtractKey, EqualKey,
    Alloc>
    const_iterator;

    typedef forward_iterator_tag    iterator_category;
    typedef Value                   value_type;
    typedef ptrdiff_t               difference;
    typedef size_t                  size_type;
    typedef Value*                  pointer;
    typedef Value&                  reference;
    typedef const Value&            const_reference;
    typedef const Value*            const_pointer;

    typedef hashtable_node<Value>   node_type;

public:
    node_type*          cur;
    hashtable_type*     table;

    operator const_iterator() {
        return const_iterator(this->cur, this->table);
    }

    hashtable_iterator() = default;

    hashtable_iterator(const const_iterator& iter)
        :cur(const_cast<node_type*>(iter.cur))
        ,table(const_cast<hashtable_type*>(iter.table))
    {}

    hashtable_iterator(node_type* node, hashtable_type* table)
        : cur(node)
        , table(table)
    {}

    bool operator==(const iterator& another) const {
        return cur == another.cur and table == another.table;
    }

    bool operator!=(const iterator& another) const {
        return !operator==(another);
    }

    const_reference operator*() const {
        return cur->data;
    }

    const_pointer operator->() const {
        return &(operator*());
    }

    reference operator*() {
        return cur->data;
    }

    pointer operator->() {
        return &(operator*());
    }

    iterator& operator++() {
        const node_type* origin = cur;
        cur = cur->next;
        if (nullptr == cur) {
            size_type bucket = table->bucket_number(origin->data);
            while (cur == nullptr and ++bucket < table->buckets.size())
                cur = table->buckets[bucket];
        }
        return *this;
    }

    iterator operator++(int) {
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }

};


template <typename Value, typename Key, typename HashFun, typename ExtractKey,
          typename EqualKey, typename Alloc = allocator<hashtable_node<Value>>>
class hash_table {
public:
    typedef HashFun             hasher;
    typedef EqualKey            key_equal;

    typedef size_t              size_type;
    typedef Value               value_type;
    typedef Key                 key_type;
    typedef value_type&         reference;
    typedef value_type*         pointer;
    typedef const value_type&   const_reference;
    typedef const value_type*   const_pointer;
    typedef ptrdiff_t           difference_type;

    typedef typename hashtable_iterator<
                               Value, Key, HashFun, ExtractKey, EqualKey,
                               Alloc>::iterator
                               iterator;
    typedef typename hashtable_const_iterator<
                               Value, Key, HashFun, ExtractKey, EqualKey,
                               Alloc>::const_iterator
                               const_iterator;
    friend iterator;
    friend const_iterator;

protected:
    typedef hashtable_node<Value>   node_type;
    typedef Alloc                   alloc;
    typedef hash_table<Value, Key, HashFun, ExtractKey, EqualKey, Alloc>
            self;

protected:

    hasher              hash_fun;
    key_equal           equals; 
    ExtractKey          get_key;
    vector<node_type*>  buckets;

    size_type           element_count;

public:
    hash_table()
        :hash_fun(hash<key_type>())
        ,equals(equal_to<key_type>())
        ,get_key(identity<key_type>())
        ,element_count(0) {
        intialize_buckets(__primes_list[0]);
    }

    hash_table(size_type bucket_size, const HashFun& hash_fun,
              const EqualKey& eql) 
        :hash_fun(hash_fun)
        ,equals(eql)
        ,get_key(ExtractKey())
        ,element_count(0) {
        intialize_buckets(bucket_size);
    }

    hash_table(const self& another)
        :hash_fun(another.hash_fun)
        ,equals(another.equals)
        ,get_key(another.get_key) {
        copy_buckets_from(another);
    }

    hash_table(self&& another) {
        hash_fun = another.hash_fun;
        equals = another.equals;
        get_key = another.get_key;
        element_count = another.element_count;

        another.element_count = 0;
        buckets = std::move(another.buckets);
    }

    self& operator=(const self& another) = default;

    self& operator=(self&& another) = default;

    size_type bucket_count() const {
        return buckets.size();
    }

    size_type max_bucket_count() const {
        return __primes_list[__num_primes-1];
    }

    size_type size() const {
        return element_count;
    }

    bool empty() const {
        return element_count == 0;
    }

    const_iterator cbeing() const {
        return const_iterator(first_node(), this);
    }

    const_iterator cend() const {
        return const_iterator(nullptr, this);
    }

    const_iterator find(const key_type& key) const {
        size_type bkt = bucket_number_key(key);
        node_type* node = nullptr;
        for (node = buckets[bkt];
             node != nullptr and  !equals(get_key(node->data), key);
             node = node->next);
        return const_iterator(node, this);
    }

    size_type count(const key_type& key) const {
        size_type number = 0;
        size_type bkt = bucket_number_key(key);
        node_type* node = nullptr;
        for (node = buckets[bkt];
             node != nullptr and !equals(get_key(node->data), key);
             node = node->next);
        if (node == nullptr)
            return 0;

        for (; node and equals(get_key(node->data), key); node = node->next)
            ++number;
        return number;
    }

    hasher hash_function() const {
        return hash_fun;
    }

    key_equal key_eq() const {
        return equals;
    }

    void swap(self& another) {
        swap(hash_fun, another.hash_fun);
        swap(equals, another.equals);
        swap(get_key, another.get_key);
        swap(element_count, another.element_count);

        buckets.swap(another.buckets);
    }

    void clear() {
        node_type* node = nullptr;
        node_type* next = nullptr;
        for (size_t i = 0; i < buckets.size(); ++i) {
            node = buckets[i];
            if (node == nullptr)
                continue;
            next = node->next;
            while (node) {
                destroy_node(node);
                node = next;
                if (next != nullptr)
                    next = next->next;
            }
        }
        copy(buckets.begin(), buckets.end(), nullptr);
        element_count = 0;
    }

    pair<iterator, bool> insert_unique(const value_type& obj) {
        rehash(element_count + 1);
        return insert_unique_no_rehash(obj);
    }

    pair<iterator, bool> insert_unique_no_rehash(const value_type& obj) {
        size_type bkt = bucket_number(obj);
        for (node_type* node = buckets[bkt]; node; node = node->next) {
            if (equals(get_key(node->data), get_key(obj))) {
                return make_pair(iterator(node, this), false);
            }
        }

        node_type* node = create_node(obj);
        node_type* go = buckets[bkt];
        node->next = go;
        buckets[bkt] = node;
        ++element_count;
        return make_pair(iterator(node, this), true);
    }

    iterator insert_equal(const value_type& obj) {
        rehash(element_count+1);
        return insert_equal_no_rehash(obj);
    }

    iterator insert_equal_no_rehash(const value_type& obj) {
        size_type bkt = bucket_number(obj);
        node_type* prev = nullptr;
        for (node_type* node = buckets[bkt]; node; node = node->next) {
            if (equals(get_key(node->data), get_key(obj))) {
                node_type* new_node = create_node(obj);
                new_node->next = node->next;
                node->next = new_node;
                ++element_count;
                return iterator(new_node, this);
            }
        }

        node_type* node = create_node(obj);
        node->next = buckets[bkt];
        buckets[bkt] = node;
        ++element_count;
        return iterator(node);
    }

    void rehash(size_type size_hint) {
        if (size_hint < buckets.size() or size_hint < element_count)
            return;
        size_type new_size = next_size(size_hint);

        vector<node_type*> tmp (new_size, nullptr);
        size_type old_size = bucket_count();
        for (size_type i = 0; i < old_size; ++i) {
            node_type* node = buckets[i];
            while (node != nullptr) {
                size_type new_bucket = bucket_number(node->data, new_size);
                buckets[i] = node->next;
                node->next = tmp[new_bucket];
                tmp[new_bucket] = node;
                node = buckets[i];
            }
        }

        buckets.swap(tmp);
    }

    iterator find(const key_type& key) {
        size_type bkt = bucket_number_key(key);
        node_type* node = nullptr;
        for (node = buckets[bkt];
             node != nullptr and  !equals(get_key(node->data), key);
             node = node->next);
        return iterator(node, this);
    }

    iterator erase(const_iterator& pos) {
        iterator next_pos = pos;
        ++next_pos;
        node_type* node = pos.cur;
        size_type bkt = bucket_number(node->data);

        node_type* pre_node = nullptr;
        for (node_type* bkt_node = buckets[bkt]; bkt_node;
             pre_node = bkt_node, bkt_node = bkt_node->next) {
            if (node == bkt_node) {
                if (pre_node == nullptr) {
                    buckets[bkt] = node->next;
                    destroy_node(node);
                } else {
                    pre_node->next = node->next;
                    destroy_node(node);
                }
            }
        }
        return next_pos;
    }

    size_type erase(const key_type& key) {
        size_type erase_count = 0;
        size_type bkt = bucket_number_key(key);

        node_type* pre_node = nullptr;
        for (node_type* bkt_node = buckets[bkt]; bkt_node;
            ) {
            if (equals(key, get_key(bkt_node->data))) {
                if (pre_node == nullptr) {
                    buckets[bkt] = bkt_node->next;
                    destroy_node(bkt_node);
                } else {
                    pre_node->next = bkt_node->next;
                    destroy_node(bkt_node);
                }
                bkt_node = pre_node->next;
                ++erase_count;
            } else {
                pre_node = bkt_node;
                bkt_node = bkt_node->next;
            }
        }
        return erase_count;
    }

    iterator begin() {
        return iterator(first_node(), this);
    }

    iterator end() {
        return iterator(nullptr, this);
    }

protected:
    node_type* first_node() const {
        for (size_t i = 0; i < buckets.size(); ++i) {
            if (buckets[i] != nullptr)
                return buckets[i];
        }
        return nullptr;
    }

    size_type bucket_number(const value_type& obj, size_t n) const {
        return bucket_number_key(get_key(obj), n);
    }

    size_type bucket_number(const value_type& obj) const {
        return bucket_number_key(get_key(obj));
    }

    size_type bucket_number_key(const key_type& key) const {
        return bucket_number_key(key, bucket_count());
    }

    size_type bucket_number_key(const key_type& key, size_t n) const {
        return hash_fun(key) % n;
    }

    void copy_buckets_from(const self& another) {
        if (!empty())
            clear();
        buckets.reserve(another.buckets.size());
        copy(buckets.begin(), buckets.end(), nullptr);

        node_type* forward_node = nullptr;
        for (size_t i = 0; i < another.buckets.size(); ++i) {
            if (const node_type* node = another.buckets[i]; node != nullptr) {
                node_type* copied = create_node(node->data);
                buckets[i] = copied;

                for (node_type* next = node->next; next; node = next,
                                                         next = node->next) {
                    copied->next = create_node(next->data);
                    copied = copied->next;
                }
            }
        }

        element_count = another.element_count;
    }

    size_type next_size(size_type n) const {
        for (size_t i = 0; i < __num_primes; ++i) {
            if (__primes_list[i] > n) {
                return __primes_list[i];
            }
        }
        return __primes_list[__num_primes - 1];
    }

    void intialize_buckets(size_type size) {
        buckets.insert(buckets.begin(), size, nullptr);
    }

    node_type* create_node(const value_type& value) {
        node_type* node = alloc::allocate(1);
        node->next = nullptr;
        construct(&node->data, value);
        return node;
    }

    void destroy_node(node_type* node) {
        destroy(&node->data);
        alloc::deallocate(node);
    }


protected:
    static constexpr int __num_primes = 28;
    static constexpr unsigned long __primes_list[__num_primes] = {
        53,         97,           193,        389,        769,
        1543,       3079,         6153,       12289,      24593,
        49157,      98317,        196613,     393241,     786433,
        1572869,    3145739,      6291469,    12582917,   25165843,
        50331653,   100663319,    201326611,  402653189,  805306457,
        1610612741, 3221225473ul, 4294967291ul
    };
};

__STLL_NAMESPACE_FINISH__

#endif // HASH_TABLE_HPP
