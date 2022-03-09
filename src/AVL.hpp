#ifndef TREE_POLICY_AVL_HPP
#define TREE_POLICY_AVL_HPP

#include "trait.hpp"
#include <algorithm>

#define get_height(x) (x==this->NIL?0:x->height)
template<typename value_t>
struct AVL_node : public basic_node_trait<AVL_node<value_t>, value_t> {
    int height;
};

template<typename value_t>
struct AVL : public basic_tree<AVL<value_t>, AVL_node<value_t>, value_t> {
    using node_t = AVL_node<value_t>;

    inline void push_up(node_t *now) {
        now->height = std::max(get_height(now->L), get_height(now->R)) + 1;
    }

    inline void push_up2(node_t *now) {
        push_up(now->L);
        push_up(now->R);
        push_up(now);
    }

    inline node_t *LL(node_t *now) {
        auto ret = this->rotate_to_right(now);
        push_up2(ret);
        return ret;
    }

    inline node_t *LR(node_t *now) {
        auto ret = this->rotate_to_left(now->L);
        push_up2(ret);
        ret = this->rotate_to_right(now);
        push_up2(ret);
        return ret;
    }

    inline node_t *RL(node_t *now) {
        auto ret = this->rotate_to_right(now->R);
        push_up2(ret);
        ret = this->rotate_to_left(now);
        push_up2(ret);
        return ret;
    }

    inline node_t *RR(node_t *now) {
        auto ret = this->rotate_to_left(now);
        push_up2(ret);
        return ret;
    }

    inline node_t *L(node_t *now) {
        node_t *L = now->L;
        int l_height = get_height(L->L);
        int r_height = get_height(L->R);
        if (l_height < r_height) {
            return LR(now);
        } else {
            return LL(now);
        }
    }

    inline node_t *R(node_t *now) {
        node_t *R = now->R;
        int l_height = get_height(R->L);
        int r_height = get_height(R->R);
        if (l_height < r_height) {
            return RR(now);
        } else {
            return RL(now);
        }
    }

    inline void rebuild_after_insert(node_t *now) {
        while (now != this->head) {
            int l = get_height(now->L);
            int r = get_height(now->R);
            int dst = std::max(l, r) + 1;
            if (now->height == dst) {
                break;
            }
            now->height = dst;
            int o = l - r;
            if (o <= -2) {
                now = R(now);
            } else if (o >= 2) {
                now = L(now);
            }
            now = now->fa;
        }

    }

    inline void rebuild_after_remove(node_t *now) {
        while (now != this->head) {
            int l = get_height(now->L);
            int r = get_height(now->R);
            int dst = std::max(l, r) + 1;
            int o = l - r;
            if (now->height == dst && o >= -1 && o <= 1) {
                break;
            }
            now->height = dst;
            if (o <= -2) {
                now = R(now);
            } else if (o >= 2) {
                now = L(now);
            }
            now = now->fa;
        }
    }

    void insert_impl(const value_t &val) {
        node_t *now = this->_insert_impl(val);
        now->height = 1;
        rebuild_after_insert(now->fa);
    }

    void erase_impl(const value_t &val) {
        node_t *beg = this->_erase_impl(val);
        if (beg == this->NIL)return;
        push_up(beg);
        rebuild_after_remove(beg);
    }

    node_t *find_impl(const value_t &val) {
        return this->_find_impl(val);
    }

    void walk_impl() {
        return this->_walk_impl();
    }
};

#endif

