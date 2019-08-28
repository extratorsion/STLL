#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <initializer_list>

#include "allocator.hpp"
#include "iterator.hpp"


__STLL_NAMESPACE_START__

namespace
{
enum {DEEUE_BUFFER_SIZE = 256};
}

template<typename Tp, typename Alloc=allocator<Tp>>
class deque {
public:
    typedef Tp           value_type;
    typedef Tp*          pointer;
    typedef const Tp*    const_pointer;
    typedef Tp&          reference;
    typedef const Tp&    const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

    typedef pointer*     map_pointer;
    typedef Alloc        alloc;

    typedef deque<Tp, Alloc>     self;

public:
    class iterator {
    public:
        typedef random_access_iterator_tag   iterator_category;
        typedef Tp                           value_type;
        typedef Tp*                          pointer;
        typedef Tp&                          reference;
        typedef size_t                       size_type;
        typedef ptrdiff_t                    difference_type;

    public:
        iterator() = default;

        iterator(map_pointer _node, pointer* _cur=nullptr) {
            node = _node;
            cur = (_cur ? _cur: *node);
            start = *node;
            finish = start + buffer_size();
        }

        reference operator*() {
            return *cur;
        }

        pointer operator->() {
            return cur;
        }

        iterator operator+(const difference_type& size) const {
            iterator iter = *this;
            if (cur + size >= start && cur + size < finish) {
                iter.cur = cur + size;
            } else if (size > 0) {
                size_t cur_node_left = finish - cur;
                size_t node_draft = (size - cur_node_left) / buffer_size()
                        + 1 - !((size - cur_node_left) % buffer_size())
                        ;
                size_t final_node_draft = size
                        - ((node_draft-1) * buffer_size())
                        - cur_node_left;

                iter.node = node + node_draft;
                iter.cur = *iter.node + final_node_draft;
                iter.start = *iter.node;
                iter.finish = iter.start + buffer_size();
            } else {
                size_t cur_node_advaced = cur - start;
                size_t node_draft = (size - cur_node_advaced) / buffer_size()
                        + 1 - !((size - cur_node_advaced) % buffer_size());
                size_t final_node_draft = size
                        - ((node_draft-1) * buffer_size())
                        - cur_node_advaced;

                iter.node = node - node_draft;
                iter.start = (*iter.node);
                iter.finish = iter.start + buffer_size();
                iter.cur = iter.finish - final_node_draft;
            }
            return iter;
        }

        iterator& operator+=(const difference_type& size) {
            iterator& iter = *this;
            if (cur + size >= start && cur + size < finish) {
                iter.cur = cur + size;
            } else if (size > 0) {
                size_t cur_node_left = finish - cur;
                size_t node_draft = (size - cur_node_left) / buffer_size()
                        + 1 - !((size - cur_node_left) % buffer_size())
                        ;
                size_t final_node_draft = size
                        - ((node_draft-1) * buffer_size())
                        - cur_node_left;

                iter.node = node + node_draft;
                iter.cur = *iter.node + final_node_draft;
                iter.start = *iter.node;
                iter.finish = iter.start + buffer_size();
            } else {

                size_t cur_node_advaced = cur - start;
                size_t node_draft = ((-size) - cur_node_advaced) / buffer_size()
                        + 1 - !((size - cur_node_advaced) % buffer_size())
                        ;
                size_t final_node_draft = (-size)
                        - ((node_draft-1) * buffer_size())
                        - cur_node_advaced;

                iter.node = node - node_draft;
                iter.start = *iter.node;
                iter.finish = iter.start + buffer_size();
                iter.cur = iter.finish - final_node_draft;
            }
            return iter;
        }

        iterator operator-(const difference_type& size) const {
            return operator+(-size);
        }

        iterator operator-=(const difference_type& size) {
            return operator+=(-size);
        }

        iterator& operator++() {
            return operator+=(1);
        }

        iterator operator++(int) {
            iterator iter = *this;
            operator+=(1);
            return iter;
        }

        iterator& operator--() {
            return operator+=(-1);
        }

        iterator operator--(int) {
            iterator iter = *this;
            operator+=(-1);
            return iter;
        }

        bool operator<(const iterator& iter) const {
            return (node < iter.node || (node == iter.node && cur < iter.cur));
        }

        bool operator>(const iterator& iter) const {
            return (node > iter.node || (node == iter.node && cur > iter.cur));
        }

        difference_type operator-(const iterator& iter) const {
            difference_type distance = 0;
            if (iter.node == node) {
                return cur - iter.cur;
            } else if (this->operator<(iter)) {
                return -iter.operator-(*this);
            } else {
                distance += (cur - start);
                distance += (iter.finish - iter.cur);
                distance += buffer_size() * ((node - iter.node) - 1);
            }
            return distance;
        }

        bool operator==(const iterator& iter) const {
            return start == iter.start && finish == iter.finish
                   && cur == iter.cur && node == iter.node;
        }

        bool operator!=(const iterator& iter) const {
            return !operator==(iter);
        }

        size_type buffer_size() const {
            return DEEUE_BUFFER_SIZE;
        }

        void set_node(map_pointer _node) {
            node = _node;
            start = *node;
            finish = start + buffer_size();

            cur = start;
        }

    protected:
        pointer start;
        pointer finish;
        pointer cur;
        map_pointer node;

        friend typename deque;
    };

protected:
    map_pointer map;
    size_type map_size;
    iterator start;
    iterator finish;

public:
    deque() {
        create_map(0);
    }

    template <typename InputIterator>
    deque(InputIterator first, InputIterator last)
        :deque() {
        while (first != last) {
            push_back(first);
        }
    }

    deque(const std::initializer_list<Tp>& initializer_list)
        :deque(initializer_list.begin(), initializer_list.end())
    {}

    deque(const self& deq) {
        map = new pointer[deq.map_size];
        map_size = deq.map_size;
        size_type start_node_index = deq.start.node - deq.map;
        size_type finish_node_index = deq.finish.node - deq.map;

        for (size_type index = start_node_index; index <= finish_node_index;
             ++index)
        {
            map[index] = allocate_node();
            copy(deq.map[index], deq.map[index] + buffer_size(), map[index]);
        }

        size_type start_iter_draft = deq.start.cur - *(deq.start.node);
        size_type finish_iter_draft = deq.finish.cur - *(deq.finish.node);
        start.set_node(deq.map + start_node_index);
        start.cur = *start.node + start_iter_draft;
        finish.set_node(deq.map + finish_node_index);
        finish.cur = *finish.node + finish_iter_draft;
    }

    deque(self&& deq) {
        start = deq.start;
        finish = deq.finish;
        map = deq.map;
        map_size = deq.map_size;

        deq.start.cur = nullptr;
        deq.finish.cur = nullptr;
        deq.map = nullptr;
        deq.map_size = 0;
    }

    ~deque() {
        clear();
    }


    self& operator=(const deque<Tp>& deq) {
        clear();
        map = new pointer[deq.map_size];
        map_size = deq.map_size;
        size_type start_node_index = deq.start.node - deq.map;
        size_type finish_node_index = deq.finish.node - deq.map;
        for (size_type index = start_node_index; index <= finish_node_index;
             ++index) {
            map[index] = allocate_node();
            copy(deq.map[index], deq.map[index] + buffer_size(), map[index]);
        }
        size_type start_iter_draft = deq.start.cur - *(deq.start.node);
        size_type finish_iter_draft = deq.finish.cur - *(deq.finish.node);
        start.set_node(deq.map + start_node_index);
        start.cur = *start.node + start_iter_draft;
        finish.set_node(deq.map + finish_node_index);
        finish.cur = *finish.node + finish_iter_draft;
        return *this;
    }

    void clear() {
        if (start.cur && finish.cur)
            for (map_pointer _node = start.node; _node <= finish.node; ++_node)
                deallocate_node(*_node);
        if (map)
            delete []map;
        map_size = 0;
    }

    const value_type& operator[](size_type index) const {
        return *(start + difference_type(index));
    }

    value_type& operator[](size_type index) {
        return *(start + difference_type(index));
    }

    void push_back(const value_type& value) {
        if (finish.cur != finish.finish - 1) {
            new(finish.cur)value_type(value);
            ++finish.cur;
        } else {
            reserve_map_at_back();
            new(finish.cur)value_type(value);
            ++finish;
        }
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (finish.cur != finish.finish - 1) {
            new(finish.cur)value_type(args...);
            ++finish.cur;
        } else {
            reserve_map_at_back();
            new(finish.cur)value_type(args...);
            ++finish;
        }
    }

    void push_front(const value_type& value) {
        if (start.cur != start.start) {
            --start.cur;
            new (start.cur)value_type(value);
        } else {
            reserve_map_at_front();
            --start;
            new (start.cur)value_type(value);
        }
    }

    template <typename... Args>
    void push_front(const Args&... args) {
        if (start.cur != start.start) {
            --start.cur;
            new (start.cur)value_type(args...);
        } else {
            reserve_map_at_front();
            --start;
            new (start.cur)value_type(args...);
        }
    }

    void swap(self& another) {
        swap(start, another.start);
        swap(finish, another.finish);
        swap(map, another.map);
        swap(map_size, another.map_size);
    }

    value_type& front() {
        return *start;
    }

    const value_type& front()const {
        return *start;
    }

    value_type& back() {
        return *(finish-1);
    }

    const value_type& back() const {
        return *(finish-1);
    }

    void pop_back() {
        iterator last = finish-1;
        if (finish.cur == finish.start)
            deallocate_node(*finish.node);
        (&*last)->~value_type();
        finish = last;
    }

    void pop_front() {
        iterator first = start;
        (&*first)->~value_type();
        if (first.cur -1 == start.finish)
            deallocate_node(*first.node);
        ++start;
    }

    bool empty() const {
        return start == finish;
    }

    size_type size() const {
        return size_type(finish - start);
    }

    size_type buffer_size() const {
        return DEEUE_BUFFER_SIZE;
    }

    iterator begin() {
        return start;
    }

    iterator end() {
        return finish;
    }


protected:
    void refill_map() {
        map_size;
        map_pointer new_map = new pointer[(map_size << 1)];
        map_pointer p_new_map = new_map + (map_size >> 1);

        for (map_pointer p_map = map; p_map < map + map_size; ++p_map) {
            *p_new_map = *p_map;
            if (p_map == start.node) {
                start.set_node(p_new_map);
            }
            if (p_map == finish.node) {
                finish.set_node(p_new_map);
            }
            ++p_new_map;
        }
        delete []map;
        map = new_map;
    }

    void reserve_map_at_back() {
        if (finish.node + 1 >= (map + map_size)) {
            refill_map();
        }
        *(finish.node + 1) = allocate_node();
    }

    void reserve_map_at_front() {
        if (start.node - 1 <= map) {
            refill_map();
        }
        *(start.node - 1) = allocate_node();
    }

    pointer allocate_node() {
        return  alloc::allocate(buffer_size());
    }

    void deallocate_node(pointer node) {
        alloc::eallocate(node, buffer_size());
    }

    void create_map(size_type size) {
        size_type node_num = size / (buffer_size()) + 1;
        size_type max_node_number =
            (8 >  (node_num << 1) ? 8 : (node_num << 1));

        map = new pointer[max_node_number];
        map_size = max_node_number;

        map_pointer start_node = map + (map_size - node_num) / 2;
        map_pointer finish_node = start_node + node_num - 1;

        map_pointer p_node = start_node;
        for (; p_node <= finish_node; ++p_node) {
            *p_node = allocate_node();
        }

        start.set_node(start_node);
        finish.set_node(finish_node);
    }

};

__STLL_NAMESPACE_FINISH__

#endif // DEQUE_HPP
