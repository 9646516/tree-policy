#ifndef TREE_POLICY_TREAP_H
#define TREE_POLICY_TREAP_H

#include "trait.hpp"
#include <algorithm>
#include <random>

template<typename value_t>
struct Treap_node : public basic_node_trait<Treap_node<value_t>, value_t> {
    uint32_t rank;

    Treap_node() {
        static std::mt19937 rng(std::random_device{}());
        rank = rng();
    }
};

//small rank in on the heap top
template<typename value_t>
struct Treap : public basic_tree<Treap<value_t>, Treap_node<value_t>, value_t> {
    using node_t = Treap_node<value_t>;

    inline void rebuild_after_insert(node_t *now) {
        while (now != this->head) {
            if (now->L != this->NIL && now->rank > now->L->rank) {
                now = this->rotate_to_right(now);
            } else if (now->R != this->NIL && now->rank > now->R->rank) {
                now = this->rotate_to_left(now);
            } else {
                break;
            }
            now = now->fa;
        }
    }

    void insert_impl(const value_t &val) {
        node_t *now = this->_insert_impl(val);
        rebuild_after_insert(now->fa);
    }

    void erase_impl(const value_t &val) {
        node_t *to_remove = this->find(val);
        if (to_remove == this->NIL)return;
        while (true) {
            if (to_remove->L == this->NIL && to_remove->R == this->NIL) {
                if (to_remove->fa->L == to_remove) {
                    to_remove->fa->L = this->NIL;
                } else {
                    to_remove->fa->R = this->NIL;
                }
                delete to_remove;
                break;
            } else if (to_remove->L != this->NIL && to_remove->R != this->NIL) {
                if (to_remove->L->rank < to_remove->R->rank) {
                    this->rotate_to_right(to_remove);
                } else {
                    this->rotate_to_left(to_remove);
                }
            } else if (to_remove->L != this->NIL) {
                if (to_remove->fa->L == to_remove) {
                    to_remove->fa->L = to_remove->L;
                } else {
                    to_remove->fa->R = to_remove->L;
                }
                to_remove->L->fa = to_remove->fa;
                delete to_remove;
                break;
            } else {
                if (to_remove->fa->L == to_remove) {
                    to_remove->fa->L = to_remove->R;
                } else {
                    to_remove->fa->R = to_remove->R;
                }
                to_remove->R->fa = to_remove->fa;
                delete to_remove;
                break;
            }
        }
    }

    node_t *find_impl(const value_t &val) {
        return this->_find_impl(val);
    }

    void walk_impl() {
        static std::function<std::string(node_t *)> F = [](node_t *now) -> std::string {
            std::stringstream ss;
            ss << now->val;
            ss << "(" << (now->rank) << ")";
            std::string ret;
            ss >> ret;
            return ret;
        };
        return this->_walk_impl(std::make_optional(F));
    }

    inline bool test_treap() {
        if (this->head->R == this->NIL)return true;
        bool ok = true;
        std::function<void(node_t *)> dfs = [&](node_t *now) {
            if (!ok)return;
            if (now->L != this->NIL) {
                if (now->L->rank < now->rank)ok = false;
                dfs(now->L);
            }
            if (now->R != this->NIL) {
                if (now->R->rank < now->rank)ok = false;
                dfs(now->R);
            }
        };
        dfs(this->head->R);
        return ok;
    }

    bool test_impl() {
        return this->test_bst() && test_treap();
    }
};

#endif