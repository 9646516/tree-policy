#ifndef TREE_POLICY_AVL_HPP
#define TREE_POLICY_AVL_HPP

#include "trait.hpp"
#include <algorithm>

#define get_height(x) (x==this->NIL?0:x->height)
template<typename value_t>
struct AVL_node : public basic_node<AVL_node<value_t>, value_t> {
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
        if (l_height <= r_height) {
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
            if (now->height == dst && (o == -1 || o == 1 || o == 0)) {
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
        auto *to_remove = this->find(val);
        node_t *ret = this->NIL;
        if (to_remove != this->NIL) {
            auto *&fa_link_bind = (to_remove->fa->L == to_remove) ? to_remove->fa->L : to_remove->fa->R;

            if (to_remove->L == this->NIL && to_remove->R == this->NIL) {
                ret = to_remove->fa;
                fa_link_bind = this->NIL;
            } else if (to_remove->L != this->NIL && to_remove->R != this->NIL) {
                node_t *next = this->find_minimum_in_sub_tree(to_remove->R);
                if (next == to_remove->R) {
                    ret = next;

                    next->height = to_remove->height;
                    next->L = to_remove->L;
                    to_remove->L->fa = next;
                    next->fa = to_remove->fa;
                    fa_link_bind = next;
                } else {
                    ret = next->fa;

                    next->height = to_remove->height;

                    next->L = to_remove->L;
                    to_remove->L->fa = next;

                    node_t *R = next->R;

                    next->R = to_remove->R;
                    to_remove->R->fa = next;

                    next->fa->L = R;
                    R->fa = next->fa;

                    next->fa = to_remove->fa;
                    fa_link_bind = next;
                }
            } else if (to_remove->L != this->NIL) {
                ret = to_remove->fa;
                to_remove->L->fa = to_remove->fa;
                fa_link_bind = to_remove->L;
            } else {
                ret = to_remove->fa;
                to_remove->R->fa = to_remove->fa;
                fa_link_bind = to_remove->R;
            }
            delete to_remove;
        }
        if (ret != this->NIL) {
            rebuild_after_remove(ret);
        }
    }

    node_t *find_impl(const value_t &val) {
        return this->_find_impl(val);
    }

    void walk_impl() {
        static std::function<std::string(node_t *)> F = [](node_t *now) -> std::string {
            std::stringstream ss;
            ss << now->val;
            ss << "(" << now->height << ")";
            std::string ret;
            ss >> ret;
            return ret;
        };
        return this->_walk_impl(std::make_optional(F));
    }

    inline bool test_avl() {
        if (this->head->R == this->NIL)return true;
        bool ok = true;
        std::function<int32_t(node_t *)> dfs = [&](node_t *now) -> int32_t {
            if (now->L == this->NIL && now->R == this->NIL) {
                return 1;
            } else {
                int left_height = (now->L == this->NIL ? 0 : dfs(now->L));
                int right_height = (now->R == this->NIL ? 0 : dfs(now->R));
                int o = left_height - right_height;
                if (o < -1 || o > 1) {
                    std::cout << now->val << ' ' << now->L->val << ' ' << now->R->val << std::endl;
                    std::cout << left_height << ' ' << right_height << std::endl;
                    ok = false;
                }
                return std::max(left_height, right_height) + 1;
            }
        };
        dfs(this->head->R);
        return ok;
    }

    bool test_impl() {
        return this->test_bst() && test_avl();
    }
};

#endif

