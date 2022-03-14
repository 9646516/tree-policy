#ifndef TREE_POLICY_SPLAY_HPP
#define TREE_POLICY_SPLAY_HPP

#include "basic_tree.hpp"

template<typename value_t>
struct Splay_node : public basic_node<Splay_node<value_t>, value_t> {
};

template<typename value_t>
struct Splay : public basic_tree<Splay<value_t>, Splay_node<value_t>, value_t> {
    using node_t = Splay_node<value_t>;

    inline bool is_left_son(node_t *now) {
        return now->fa->L == now;
    }

    inline void splay(node_t *now) {
        if (now == this->NIL)return;
        while (true) {
            if (now->fa == this->head)break;
            bool now_is_left = is_left_son(now);
            if (now->fa->fa == this->head) {
                if (now_is_left) {
                    this->rotate_to_right(now->fa);
                } else {
                    this->rotate_to_left(now->fa);
                }
            } else {
                bool fa_is_left = is_left_son(now->fa);
                if (now_is_left == fa_is_left) {
                    if (now_is_left) {
                        this->rotate_to_right(now->fa->fa);
                        this->rotate_to_right(now->fa);
                    } else {
                        this->rotate_to_left(now->fa->fa);
                        this->rotate_to_left(now->fa);
                    }
                } else {
                    if (now_is_left) {
                        this->rotate_to_right(now->fa);
                        this->rotate_to_left(now->fa);
                    } else {
                        this->rotate_to_left(now->fa);
                        this->rotate_to_right(now->fa);
                    }
                }
            }
        }
    }

    inline void merge(node_t *L, node_t *R) {
        if (L == this->NIL) {
            this->head->R = R;
            R->fa = this->head;
        } else if (R == this->NIL) {
            this->head->R = L;
            L->fa = this->head;
        } else {
            this->head->R = L;
            L->fa = this->head;
            node_t *max_node = this->find_maximum_in_sub_tree(L);
            splay(max_node);
            max_node->R = R;
            R->fa = max_node;
        }
    }

    void insert_impl(const value_t &val) {
        node_t *now = this->_insert_impl(val);
        splay(now);
    }

    void erase_impl(const value_t &val) {
        node_t *now = this->find(val);
        if (now == this->NIL)return;
        splay(now);
        merge(now->L, now->R);

        delete now;
    }

    node_t *find_impl(const value_t &val) {
        node_t *ret = this->_find_impl(val);
        splay(ret);
        return ret;
    }

    void walk_impl() {
        return this->_walk_impl();
    }

    bool test_impl() {
        return this->test_bst();
    }
};

#endif
