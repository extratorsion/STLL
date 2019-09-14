#ifndef RBTREE_HPP
#define RBTREE_HPP

#include "iterator.hpp"
#include "memory.hpp"
#include "utility.hpp"
#include "vector.hpp"
#include "functor.hpp"

__STLL_NAMESPACE_START__

namespace
{

// enum Color {BLACK=true, RED=false};
using rb_tree_color         = bool;
const rb_tree_color BLACK   = true;
const rb_tree_color RED     = false;

struct rb_tree_base_node {
    typedef rb_tree_color        color_type;
    typedef rb_tree_base_node*   base_ptr;

    base_ptr        parent;
    base_ptr        left;
    base_ptr        right;
    color_type      color;

    /*
    rb_tree_node(base_ptr parent=nullptr, base_ptr left=nullptr,
           base_ptr right=nullptr, rb_tree_color color=RED)
        :parent(parent), left(left), right(right), color(color)
    {}
    */
    base_ptr predecessor(const base_ptr Nil=nullptr) const {
        if (left != Nil) {
            base_ptr node = left;
            while (node->right != Nil)
                node = node->right;
            return node;
        } else {
            base_ptr node = const_cast<base_ptr>(this);
            base_ptr par = node->parent;
            while (par != Nil and node == par->left) {
                node = par;
                par = par->parent;
            }
            return par;
        }
    }

    base_ptr successor(const base_ptr Nil=nullptr) const {
        if (right != Nil) {
            base_ptr node = right;
            while (node->left != Nil)
                node = node->left;
            return node;
        } else {
            base_ptr node = const_cast<base_ptr>(this);
            base_ptr par = node->parent;
            while (par != Nil and node == par->right) {
                node = par;
                par = par->parent;
            }
            return par;
        }
    }

    bool is_left_child(const base_ptr Nil=nullptr) const {
        return parent != Nil and parent->left == this;
    }

    bool is_right_child(const base_ptr Nil=nullptr) const {
        return parent != Nil and parent->right == this;
    }

    base_ptr left_sibiling() const {
        return parent->left;
    }

    base_ptr right_sibling() const {
        return parent->right;
    }

};

static rb_tree_base_node NIL_OBJ{nullptr, nullptr, nullptr, BLACK};
static rb_tree_base_node* NIL = &NIL_OBJ;

template <typename Tp>
struct rb_tree_node : public rb_tree_base_node {
    typedef rb_tree_node<Tp>* link_type;
    Tp value_field;
};


struct rb_tree_base_iterator  {
    typedef typename rb_tree_base_node::base_ptr    base_ptr;
    typedef bidirectional_iterator_tag              iterator_category;
    typedef ptrdiff_t                               difference_type;
    typedef size_t                                  size_type;

    rb_tree_base_node* node;

    void increment() {
        node = node->successor(NIL);
    }

    void decrement() {
        node = node->predecessor(NIL);
    }

    bool operator==(const rb_tree_base_iterator& other) {
        return node == other.node;
    }

    bool operator!=(const rb_tree_base_iterator& other) {
        return !operator==(other);
    }
};

template <typename Tp, typename Ref, typename Ptr>
struct rb_tree_iterator : public rb_tree_base_iterator {
    typedef typename rb_tree_node<Tp>::link_type    link_type;
    typedef Tp                                      value_type;
    typedef Ref                                     reference;
    typedef Ptr                                     pointer;

    typedef rb_tree_iterator<Tp, const Tp&, const Tp*>
                                                    const_iterator;
    typedef rb_tree_iterator<Tp, Tp&, Tp*>          iterator;

    typedef rb_tree_iterator<Tp, Ref, Ptr>          self;

    reference operator*() const {
        return (link_type(node))->value_field;
    }

    pointer operator->() const {
        return &(operator*());
    }

    self& operator++() {
        this->increment();
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--() {
        this->decrement();
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }

};

}


template <typename Key, typename Value=Key, typename KeyOfValue=identity<Key>,
          typename Compare=less<Key>,
          typename Alloc=allocator<rb_tree_node<Value>> >
class rb_tree {
protected:
    typedef void*               void_pointer;
    typedef rb_tree_base_node*  base_ptr;
    typedef rb_tree_node<Value> tree_node;
    typedef Alloc               alloc;
    typedef rb_tree<Key, Value, KeyOfValue, Compare, Alloc>
                                self;

public:
    typedef Key                 key_type;
    typedef Value               value_type;
    typedef value_type*         pointer;
    typedef value_type&         reference;
    typedef const value_type*   const_pointer;
    typedef const value_type&   const_reference;
    typedef tree_node*          link_type;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

    typedef rb_tree_iterator<value_type, reference, pointer>
                                iterator;

    typedef rb_tree_iterator<value_type, const_reference, const_pointer>
                                const_iterator;
protected:
    size_type   node_count;
    link_type   tree_root;
    Compare     compare;

public:
    rb_tree()
        : node_count(0)
        , tree_root(link_type(NIL))
        , compare(Compare())
    {}

    rb_tree(const Compare& compare)
        : node_count(0)
        , tree_root(link_type(NIL))
        , compare(compare)
    {}

    rb_tree(const self& other) {
        reflect_copy(other.root(), tree_root);
    }

    rb_tree(self&& other) {
        tree_root = other.tree_root;
        node_count = other.node_count;
        compare = other.compare;

        other.tree_root = NIL;
        other.node_count = 0;
    }

    ~rb_tree() {
        clear();
    }

    self& operator=(const self& other) {
        clear();
        reflect_copy(other.root(), tree_root);
        return *this;
    }

    self& operator=(self&& other) {
        clear();
        tree_root = other.tree_root;
        node_count = other.node_count;
        compare = other.compare;
        other.tree_root = NIL;
        other.node_count = 0;
    }

    const link_type& root() const {
        return tree_root;
    }

    size_type size() const {
        return node_count;
    }

    bool empty() const {
        return node_count == 0;
    }

    size_type max_size() const {
        return size_type(-1);
    }

    size_type count(const value_type& value) const {
        link_type ret = search_in(value);
        if (ret == NIL or
                (compare(ret->value_field, value) or
                 compare(value, ret->value_field)))
            return 0;
        else
            return 1;
    }

    const_iterator find(const value_type& value) const {
        link_type ret = search_in(value);
        if (ret == NIL or
                (compare(ret->value_field, value) or
                 compare(value, ret->value_field)))
            return const_iterator{NIL};
        else
            return const_iterator{ret};
    }

    const_iterator begin() const {
        const_iterator iter {most_left()};
        return iter;
    }

    const_iterator end() const {
        const_iterator iter {NIL};
        return iter;
    }

    const Compare& key_comp() const {
        return compare;
    }

    iterator find(const value_type& value)  {
        link_type ret = search_in(value);
        if (ret == NIL or
                (compare(ret->value_field, value) or
                 compare(value, ret->value_field)))
            return iterator{NIL};
        else
            return iterator{ret};
    }


    void clear() {
         vector<link_type> nodes(node_count);
         for (iterator iter = begin(); iter != end(); ++iter)
             nodes.push_back(link_type(iter.node));
         for (auto node: nodes)
             destroy_node(node);
         this->tree_root = link_type(NIL);
         node_count = 0;
    }

    void swap(self& other) {
        swap(tree_root, other.tree_root);
        swap(compare, other.compare);
        swap(node_count, other.node_count);
    }

    pair<iterator, bool> insert(const value_type& value) {
        link_type ret = search_in(value);
        if (ret == NIL) {
            this->tree_root = create_node(value);
            this->tree_root->color = BLACK;
            ++node_count;
            return {iterator{this->root()}, true};
        } else if (!compare(ret->value_field, value)
                   and
                   !compare(value, ret->value_field)
                   ) {
            return {iterator{ret}, false};
        } else {
            link_type new_node = create_node(value);
            new_node->parent = ret;
            if (compare(value, ret->value_field)) {
                ret->left = new_node;
            } else {
                ret->right = new_node;
            }
            fixup(new_node);
            ++node_count;
            return {iterator{new_node}, true};
        }
    }

    pair<iterator, bool> insert(iterator& /*pos*/,
                                      const value_type& value) {
        return insert(value);
    }

    size_type erase(const value_type& value) {
        link_type ret = search_in(value);
        if (ret == NIL or
                (compare(ret->value_field, value) or
                 compare(value, ret->value_field)))
            return 0;
        else {
            remove_node_switch(ret);
            --node_count;
            return 1;
        }
    }

    iterator erase(const_iterator& pos) {
        link_type next_node = link_type(base_ptr(pos.node)->successor(NIL));
        remove_node_switch(pos.node);
        iterator iter;
        iter.node = next_node;
        --node_count;
        return iter;
    }

    iterator begin()  {
        iterator iter {most_left()};
        return iter;
    }

    iterator end()  {
        iterator iter {NIL};
        return iter;
    }


protected:
    link_type most_left() const {
        if (tree_root == link_type(NIL))
            return link_type(NIL);
        link_type node = tree_root;
        while (node->left != NIL) {
            node = link_type(node->left);
        }
        return node;
    }

    link_type get_node() {
        return alloc::allocate(1);
    }

    link_type clone_node(link_type x) {
        link_type tmp = create_node(x->value_field);
        tmp->color = x->color;
        tmp->left = tmp->right = tmp->parent = NIL;
        return tmp;
    }

    void put_node(link_type ptr) {
        alloc::deallocate(ptr, 1);
    }

    link_type create_node(const value_type& x) {
        link_type tmp = get_node();
        tmp->color = RED;
        tmp->left = tmp->right = tmp->parent = NIL;
        construct(&tmp->value_field, x);
        return tmp;
    }

    void reflect_copy(const link_type& from, link_type& to) {
        if (from == NIL)
            return;
        if (to == NIL)
            to = clone_node(from);
        if (from->left != NIL) {
            copy(from->left, to->left);
            to->left->parent = to;
        }
        if (from->right != NIL) {
            copy(from->right, to->right);
            to->right->parent = to;
        }
    }


    void destroy_node(link_type ptr) {
        destroy(&ptr->value_field);
        put_node(ptr);
    }


    bool search(const value_type& value) const {
        link_type ret = search_in(value);
        if (ret == NIL or ret->value_field != value)
            return false;
        return true;
    }


    void fixup(base_ptr node) {
        while (node->parent->color == RED) {
            if (node->parent->is_left_child()) {
                base_ptr S = node->parent->right_sibling();

                if (S->color == BLACK) {
                    if (node->is_left_child()) {
                        node->parent->color = BLACK;
                        node->parent->parent->color = RED;
                        right_rotate(node->parent->parent);
                    } else {
                        base_ptr povit = node->parent;
                        left_rotate(povit);
                        node = povit;
                    }
                } else {
                    if (node->is_left_child()) {
                        node->parent->color = BLACK;
                        node->parent->parent->right->color = BLACK;
                        node->parent->parent->color = RED;
                        node = node->parent->parent;
                    } else {
                        base_ptr povit = node->parent;
                        left_rotate(povit);
                        node = povit;
                    }
                }
            } else {
                base_ptr S = node->parent->parent->left;

                if (S->color == BLACK) {
                    if (node->is_right_child()) {
                        node->parent->color = BLACK;
                        node->parent->parent->color = RED;
                        left_rotate(node->parent->parent);
                    } else {
                        base_ptr povit = node->parent;
                        right_rotate(povit);
                        node = povit;
                    }
                } else {
                    if (node->is_right_child()) {
                        S->color = BLACK;
                        node->parent->color = BLACK;
                        node->parent->parent->color = RED;
                        node = node->parent->parent;
                    } else {
                        base_ptr povit = node->parent;
                        right_rotate(povit);
                        node = povit;
                    }
                }
            }
        }
        this->tree_root->color = BLACK;
    }

    void transplant(base_ptr from_node, base_ptr to_node) {
        from_node->parent = to_node->parent;
        if (to_node->parent == NIL)
            this->tree_root = from_node;
        else if (to_node->parent->left == to_node) {
            to_node->parent->left = from_node;
        } else {
            to_node->parent->right = from_node;
        }
    }

    void remove_node_switch(base_ptr node) {
        // If node's left or node's right is not NIL and
        // not both are NIL.
        if ((node->left != NIL or node->right != NIL)
                and !(node->left != NIL and node->right != NIL)
                ) {
            // In if: node->left->color == RED,
            // and else:  node->right->color == RED,
            // both node->color == BLACK.
            if (node->left == NIL) {
                transplant(node->right, node);
                node->right->color = BLACK;
            } else {
                transplant(node->left, node);
                node->left->color = BLACK;
            }
        } else if (node->right != NIL) {
            base_ptr succ = node->successor(NIL);
            swap(link_type(node)->value_field, link_type(node)->value_field);
            remove_node_switch(succ);
        } else  {
            remove_node(node);
        }
    }

    void drop_node(base_ptr node) {
        if (node == this->tree_root) {
            this->tree_root = NIL;
        } else if (node->parent->left == node) {
            node->parent->left = NIL;
        } else {
            node->parent->right = NIL;
        }
        destroy_node(node);
    }

    // Node must be leaf node.
    void remove_node(base_ptr node) {
        if (node == this->tree_root or node->color == RED) {
            drop_node(node);
            return;
        }


        // If node's color is BLACK.
        if (node->is_left_child()) {
            base_ptr S = node->right_sibling();
            if (S->color == RED) {
                // node->parent->color must be black
                swap(node->parent->color, S->color);
                left_rotate(node->parent);
            } else {
                if (S->right->color == RED) {
                    base_ptr SR = S->right;
                    swap(node->parent->color, S->color);
                    SR->color = BLACK;
                    left_rotate(node->parent);
                } else if (S->left->color == RED){
                    base_ptr SL = S->left;
                    swap(SL->color, S->color);
                    right_rotate(S);
                    // --------
                    base_ptr NS = node->parent->right;
                    base_ptr NSR = NS->right;
                    swap(node->parent->color, NS->color);
                    NSR->color = BLACK;
                    left_rotate(node->parent);
                } else {
                    node->parent->color = BLACK;
                    S->color = RED;
                }
            }
        } else {
            base_ptr S = node->left_sibiling();
            if (S->color == RED) {
                swap(node->parent->color, S->color);
                right_rotate(node->parent);
            } else {
                if (S->left->color == RED) {
                    base_ptr SL = S->left;
                    swap(node->parent->color, S->color);
                    SL->color = BLACK;
                    right_rotate(node->parent);
                } else if (S->right->color == RED) {
                    base_ptr SR = S->right;
                    swap(SR->color, S->color);
                    left_rotate(S);

                    // --------
                    base_ptr NS = node->parent->left;
                    base_ptr NSL = NS->left;
                    swap(node->parent->color, NS->color);
                    NSL->color = BLACK;
                    right_rotate(node->parent);

                } else {
                    node->parent->color = BLACK;
                    S->color = RED;
                }
            }

        }
        drop_node(node);
    }

    //
    //     A(node)
    //           B
    //
    //
    void left_rotate(base_ptr node) {
        base_ptr B = node->right;
        node->right = B->left;
        if (B->left != NIL) B->left->parent = node;
        B->left = node;

        B->parent = node->parent;
        if (node->parent == NIL) {
            this->tree_root = link_type(B);
        } else if (node->parent->left == node) {
            node->parent->left = B;
        } else {
            node->parent->right = B;
        }

        node->parent = B;
    }

    //
    //     B(node)
    // A
    //
    //
    void right_rotate(base_ptr node) {
        base_ptr A = node->left;
        node->left = A->right;
        if (A->right != NIL)
            A->right->parent = node;
        A->right = node;

        A->parent = node->parent;
        if (node->parent == NIL) {
            this->tree_root = link_type(A);
        } else if (node->parent->left == node) {
            node->parent->left = A;
        } else {
            node->parent->right = A;
        }
        node->parent = A;

    }

    // Return parent if value not exist in tree
    // else return node with value_filed == value
    link_type search_in(const value_type& value) const {
        if (link_type(NIL) == root())
            return link_type(NIL);
        base_ptr p = NIL;
        base_ptr n = root();
        while (n != nullptr and n != NIL) {
            if (!compare(link_type(n)->value_field, value)
                and
                !compare(value, link_type(n)->value_field)) {
                return link_type(n);
            } else if (compare(value, link_type(n)->value_field)) {
                p = n; n = n->left;
            } else {
                p = n; n = n->right;
            }
        }
        return link_type(p);
    }

};

__STLL_NAMESPACE_FINISH__



#endif // RBTREE_HPP
