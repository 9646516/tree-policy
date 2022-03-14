#ifndef TREE_POLICY_BASIC_TREE_HPP
#define TREE_POLICY_BASIC_TREE_HPP

#include <sstream>
#include <deque>
#include "trait.hpp"
#include <optional>
#include <functional>

template<typename node_t, typename value_t>
struct basic_node {
    node_t *L, *R, *fa;
    value_t val;
};

template<typename tree_t, typename node_t, typename value_t>
struct basic_tree : public tree_trait<tree_t, node_t, value_t> {

    node_t *head;

    node_t *create_node(const value_t &val = 0) {
        auto *ret = new node_t();
        ret->L = ret->R = this->NIL;
        ret->val = val;
        return ret;
    }

    basic_tree() {
        this->NIL = new node_t();
        this->NIL->fa = this->NIL->L = this->NIL->R = this->NIL;
        this->NIL->val = 1919810;
        this->head = new node_t();
        this->head->L = this->head->R = this->NIL;
        this->head->val = 114514;
        this->head->fa = this->head;
    }

    ~basic_tree() {
        std::function<void(node_t *)> dfs = [&](node_t *now) {
            if (now == this->NIL)return;
            if (now->L != this->NIL)dfs(now->L);
            if (now->R != this->NIL)dfs(now->R);
            delete now;
        };
        dfs(head->R);
        delete this->NIL;
        delete head;
    }

    node_t *find_minimum_in_sub_tree(node_t *now) {
        while (true) {
            node_t *next = now->L;
            if (next == this->NIL) {
                break;
            } else {
                now = next;
            }
        }
        return now;
    }

    node_t *find_maximum_in_sub_tree(node_t *now) {
        while (true) {
            node_t *next = now->R;
            if (next == this->NIL) {
                break;
            } else {
                now = next;
            }
        }
        return now;
    }


    void _walk_impl(std::optional<std::function<std::string(node_t *)>> F = std::nullopt) {
        if (this->head->R == this->NIL) {
            std::cout << "NULL Tree" << std::endl;
        } else {
            static char sb[1000][1000];
            std::memset(sb, ' ', sizeof(sb));
            std::function<void(node_t *, int, int)> write = [&](node_t *now, int x, int y) {
                std::string str;
                if (F.has_value()) {
                    str = F.value()(now);
                } else {
                    std::stringstream ss;
                    ss << now->val;
                    ss >> str;
                }

                for (int i = 0; i < (int) str.size(); i++) {
                    sb[x][y + i] = str[i];
                }
            };
            std::function<void(const char *, int, int)> write_str = [&](const char *val, int x, int y) {
                int len = (int) strlen(val);
                for (int i = 0; i < len; i++) {
                    sb[x][y + i] = val[i];
                }
            };
            std::function<int(node_t *, int, int)> dfs = [&](node_t *now, int x, int y) -> int {
                if (now == this->NIL) {
                    write_str("NIL", x, y);
                    return 1;
                }
                write(now, x, y);
                sb[x + 1][y] = '|';
                for (int i = 0; i < 5; i++) {
                    sb[x + 1][y + 1 + i] = '-';
                }
                int used = dfs(now->R, x + 1, y + 6);
                for (int i = 0; i < used; i++) {
                    sb[x + 1 + 1 + i][y] = '|';
                }
                for (int i = 0; i < 5; i++) {
                    sb[x + 1 + used][y + 1 + i] = '-';
                }
                int used2 = dfs(now->L, x + 1 + used, y + 6);
                return used + used2 + 1 + 1;
            };
            dfs(head->R, 0, 0);
            for (auto &i: sb) {
                bool ok = false;
                for (char j: i) {
                    if (j != ' ') {
                        ok = true;
                        break;
                    }
                }
                if (!ok)break;
                int last = 0;
                for (int j = 999; j >= 0; j--) {
                    if (i[j] != ' ') {
                        last = j;
                        break;
                    }
                }
                for (int j = 0; j <= last; j++)
                    std::cout << i[j];
                std::cout << std::endl;
            }
        }
    }


    node_t *_find_impl(const value_t &val) {
        auto now = this->head->R;
        while (now != this->NIL) {
            if (val < now->val) {
                now = now->L;
            } else if (val > now->val) {
                now = now->R;
            } else {
                return now;
            }
        }
        return this->NIL;
    }

    node_t *_insert_impl(const value_t &val) {
        node_t *add = this->create_node(val);
        if (this->head->R == this->NIL) {
            this->head->R = add;
            add->fa = this->head;
        } else {
            auto now = this->head->R, last = this->NIL;
            while (now != this->NIL) {
                last = now;
                if (val < now->val) {
                    now = now->L;
                } else {
                    now = now->R;
                }
            }
            add->fa = last;
            if (val < last->val) {
                last->L = add;
            } else {
                last->R = add;
            }
        }
        return add;
    }

    node_t *_erase_impl(const value_t &val) {
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

                    next->L = to_remove->L;
                    to_remove->L->fa = next;
                    next->fa = to_remove->fa;
                    fa_link_bind = next;
                } else {
                    ret = next->fa;

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
        return ret;
    }

    ///          4                                   2
    ///         / \                                 / \
    ///        2   5    rotate_to_right(4)         1   4
    ///       / \                                     / \
    ///      1   3                                   3   5
    inline node_t *rotate_to_right(node_t *now) {
        node_t *left = now->L;
        node_t *fa = now->fa;

        now->L = left->R;
        left->R->fa = now;

        left->R = now;
        now->fa = left;

        if (fa->L == now) {
            fa->L = left;
        } else {
            fa->R = left;
        }
        left->fa = fa;
        return left;
    }

    inline node_t *rotate_to_left(node_t *now) {
        node_t *right = now->R;
        node_t *fa = now->fa;

        now->R = right->L;
        right->L->fa = now;

        right->L = now;
        now->fa = right;
        if (fa->L == now) {
            fa->L = right;
        } else {
            fa->R = right;
        }
        right->fa = fa;
        return right;
    }

    inline bool test_bst() {
        if (this->head->R == this->NIL)return true;
        bool ok = true;
        std::function<std::pair<value_t, value_t>(node_t *)> dfs = [&](node_t *now) -> std::pair<value_t, value_t> {
            if (!ok)return {0, 0};
            else if (now->L == this->NIL && now->R == this->NIL) {
                return {now->val, now->val};
            } else if (now->L == this->NIL) {
                auto[rmin, rmax]= dfs(now->R);
                if (now->val > rmin) {
                    ok = false;
                }
                return {now->val, rmax};
            } else if (now->R == this->NIL) {
                auto[lmin, lmax]= dfs(now->L);
                if (now->val < lmax) {
                    ok = false;
                }
                return {lmin, now->val};
            } else {
                auto[lmin, lmax] = dfs(now->L);
                auto[rmin, rmax]= dfs(now->R);
                if (!(lmax <= now->val && now->val <= rmin)) {
                    ok = false;
                }
                return {lmin, rmax};
            }
        };
        dfs(this->head->R);
        return ok;
    }

    int calc_height_impl() {
        std::function<int(node_t *)> dfs = [&](node_t *now) -> int {
            if (now == this->NIL) {
                return 0;
            } else {
                return std::max(dfs(now->L), dfs(now->R)) + 1;
            }
        };
        return dfs(this->head->R);
    }
};

#endif
