#ifndef TREE_POLICY_RBTREE_HPP
#define TREE_POLICY_RBTREE_HPP

#include <sstream>
#include "basic_tree.hpp"
#include "trait.hpp"

template<typename value_t>
struct RBTree_node : public basic_node<RBTree_node<value_t>, value_t> {
    COLOR color;
};

template<typename value_t>
struct RBTree : public basic_tree<RBTree<value_t>, RBTree_node<value_t>, value_t> {
    using node_t = RBTree_node<value_t>;

    explicit RBTree() {
        this->head->color = COLOR::BLACK;
        this->NIL->color = COLOR::BLACK;
    }

    void insert_impl(const value_t &val) {
        node_t *now = this->_insert_impl(val);
        if (now == this->head->R) {
            now->color = COLOR::BLACK;
            return;
        } else if (now->fa == this->head->R) {
            now->color = COLOR::RED;
            return;
        } else {
            now->color = COLOR::RED;
            while (now->fa->color == COLOR::RED) {
                if (now->fa->fa->L == now->fa) {
                    node_t *uncle = now->fa->fa->R;
                    if (uncle->color == COLOR::RED) {
                        now->fa->color = COLOR::BLACK;
                        uncle->color = COLOR::BLACK;
                        now->fa->fa->color = COLOR::RED;
                        now = now->fa->fa;
                    } else {
                        if (now == now->fa->R) {
                            now = now->fa;
                            this->rotate_to_left(now);
                        }
                        now->fa->color = COLOR::BLACK;
                        now->fa->fa->color = COLOR::RED;
                        this->rotate_to_right(now->fa->fa);
                    }
                } else {
                    node_t *uncle = now->fa->fa->L;
                    if (uncle->color == COLOR::RED) {
                        now->fa->color = COLOR::BLACK;
                        uncle->color = COLOR::BLACK;
                        now->fa->fa->color = COLOR::RED;
                        now = now->fa->fa;
                    } else {
                        if (now == now->fa->L) {
                            now = now->fa;
                            this->rotate_to_right(now);
                        }
                        now->fa->color = COLOR::BLACK;
                        now->fa->fa->color = COLOR::RED;
                        this->rotate_to_left(now->fa->fa);
                    }
                }
            }
        }
        this->head->R->color = COLOR::BLACK;
    }

    void erase_impl(const value_t &val) {
        auto *to_remove = this->find(val);
        node_t *now;
        COLOR origin_color;

        if (to_remove != this->NIL) {
            auto *&fa_link_bind = (to_remove->fa->L == to_remove) ? to_remove->fa->L : to_remove->fa->R;

            if (to_remove->L != this->NIL && to_remove->R != this->NIL) {
                node_t *next = this->find_minimum_in_sub_tree(to_remove->R);
                origin_color = next->color;
                next->color = to_remove->color;
                now = next->R;
                if (next == to_remove->R) {
                    now->fa = next;

                    next->L = to_remove->L;
                    to_remove->L->fa = next;
                    next->fa = to_remove->fa;
                    fa_link_bind = next;
                } else {
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
                now = to_remove->L;
                origin_color = to_remove->color;
                to_remove->L->fa = to_remove->fa;
                fa_link_bind = to_remove->L;
            } else {
                now = to_remove->R;
                origin_color = to_remove->color;
                to_remove->R->fa = to_remove->fa;
                fa_link_bind = to_remove->R;
            }
            delete to_remove;

            if (now == this->head) {
                return;
            } else if (now == this->head->R) {
                now->color = COLOR::BLACK;
                return;
            } else {
                if (origin_color == COLOR::BLACK) {
                    while (now != this->head->R && now->color == COLOR::BLACK) {
                        if (now == now->fa->L) {
                            node_t *brother = now->fa->R;
                            if (brother->color == COLOR::RED) {
                                brother->color = COLOR::BLACK;
                                now->fa->color = COLOR::RED;
                                this->rotate_to_left(now->fa);
                                brother = now->fa->R;
                            }
                            if (brother->L->color == COLOR::BLACK && brother->R->color == COLOR::BLACK) {
                                brother->color = COLOR::RED;
                                now = now->fa;
                            } else {
                                if (brother->R->color == COLOR::BLACK) {
                                    brother->L->color = COLOR::BLACK;
                                    brother->color = COLOR::RED;
                                    brother = this->rotate_to_right(brother);
                                    if (now == this->NIL)now->fa = brother->fa;
                                }
                                brother->color = now->fa->color;
                                now->fa->color = COLOR::BLACK;
                                brother->R->color = COLOR::BLACK;
                                this->rotate_to_left(now->fa);
                                now = this->head->R;
                            }
                        } else {
                            node_t *brother = now->fa->L;
                            if (brother->color == COLOR::RED) {
                                brother->color = COLOR::BLACK;
                                now->fa->color = COLOR::RED;
                                this->rotate_to_right(now->fa);
                                brother = now->fa->L;
                            }
                            if (brother->L->color == COLOR::BLACK && brother->R->color == COLOR::BLACK) {
                                brother->color = COLOR::RED;
                                now = now->fa;
                            } else {
                                if (brother->L->color == COLOR::BLACK) {
                                    brother->R->color = COLOR::BLACK;
                                    brother->color = COLOR::RED;
                                    brother = this->rotate_to_left(brother);
                                    if (now == this->NIL)now->fa = brother->fa;
                                }
                                brother->color = now->fa->color;
                                now->fa->color = COLOR::BLACK;
                                brother->L->color = COLOR::BLACK;
                                this->rotate_to_right(now->fa);
                                now = this->head->R;
                            }
                        }
                    }
                    now->color = COLOR::BLACK;
                }
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
            ss << "(" << (now->color == COLOR::BLACK ? "B" : "R") << ")";
            std::string ret;
            ss >> ret;
            return ret;
        };
        return this->_walk_impl(std::make_optional(F));
    }

    inline bool test_rbtree() {
        if (this->head->R == this->NIL)return true;
        if (this->head->R->color != COLOR::BLACK)return false;
        bool ok = true;
        std::function<int32_t(node_t *)> dfs = [&](node_t *now) -> int32_t {
            if (!ok)return 0;
            else if (now->L == this->NIL && now->R == this->NIL) {
                return now->color == COLOR::BLACK;
            } else {
                int left_height = (now->L == this->NIL ? 0 : dfs(now->L));
                int right_height = (now->R == this->NIL ? 0 : dfs(now->R));
                if (left_height != right_height) {
                    std::cout << "bh not match " << now->val << ' ' << left_height << ' ' << right_height << std::endl;
                    ok = false;
                }
                if (now->color == COLOR::RED) {
                    bool f1 = now->L != this->NIL && now->L->color == COLOR::RED;
                    bool f2 = now->R != this->NIL && now->R->color == COLOR::RED;
                    if (f1 || f2) {
                        ok = false;
                        std::cout << "double red" << ' ' << now->val;
                        if (f1)std::cout << ' ' << now->L->val << std::endl;
                        else std::cout << ' ' << now->R->val << std::endl;
                    }
                }
                return left_height + (now->color == COLOR::BLACK);
            }
        };
        dfs(this->head->R);
        return ok;
    }

    bool test_impl() {
        return this->test_bst() && test_rbtree();
    }
};

#endif
