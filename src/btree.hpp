#ifndef TREE_POLICY_BTREE_H
#define TREE_POLICY_BTREE_H

#include "trait.hpp"
#include <algorithm>
#include <random>

//2-3-4 tree
template<typename value_t>
struct BTree_node {
    value_t *val;
    BTree_node **son;
    int cnt;
    bool is_leaf;

    BTree_node() {
        val = new value_t[3];
        son = new BTree_node *[4];
        cnt = 0;
        is_leaf = true;
    }

    ~BTree_node() {
        delete[]val;
        delete[]son;
    }

    [[nodiscard]] int number_of_son() const {
        return this->cnt + 1;
    }

    [[nodiscard]] int number_of_val() const {
        return this->cnt;
    }

};

template<typename value_t>
struct BTree : public tree_trait<BTree<value_t>, BTree_node<value_t>, value_t> {
    using node_t = BTree_node<value_t>;
    node_t *root;

    node_t *create_node() {
        auto *ret = new node_t();
        for (int i = 0; i < 4; i++) {
            ret->son[i] = this->NIL;
        }
        return ret;
    }

    BTree() {
        this->NIL = new node_t();
        this->root = this->NIL;
    }

    ~BTree() {
        std::function<void(node_t *)> dfs = [&](node_t *now) {
            if (now == this->NIL)return;
            if (!now->is_leaf) {
                const int len = now->number_of_son();
                for (int i = 0; i < len; i++) {
                    dfs(now->son[i]);
                }
            }
            delete now;
        };
        dfs(root);
        delete this->NIL;
    }

    inline void son_shift_right(node_t *now, int L, int R, int offset = 1) {
        for (int i = R; i >= L; i--) {
            now->son[i + offset] = now->son[i];
        }
    }

    inline void val_shift_right(node_t *now, int L, int R, int offset = 1) {
        for (int i = R; i >= L; i--) {
            now->val[i + offset] = now->val[i];
        }
    }

    inline void son_shift_left(node_t *now, int L, int R, int offset = 1) {
        for (int i = L; i <= R; i++) {
            now->son[i - offset] = now->son[i];
        }
    }

    inline void val_shift_left(node_t *now, int L, int R, int offset = 1) {
        for (int i = L; i <= R; i++) {
            now->val[i - offset] = now->val[i];
        }
    }

    inline void split(node_t *now, int idx) {
        node_t *l_part = now->son[idx];
        int split_at = l_part->val[1];
        auto *r_part = new node_t();
        r_part->is_leaf = l_part->is_leaf;
        this->val_shift_right(now, idx, now->number_of_val() - 1);
        now->val[idx] = split_at;
        this->son_shift_right(now, idx + 1, now->number_of_son() - 1);
        now->son[idx] = l_part;
        now->son[idx + 1] = r_part;
        r_part->cnt = 1;
        r_part->val[0] = l_part->val[2];
        r_part->son[0] = l_part->son[2];
        r_part->son[1] = l_part->son[3];
        l_part->cnt = 1;
        l_part->son[2] = l_part->son[3] = this->NIL;
        now->cnt++;

    }

    //number of key word in now should be 1~2
    void insert_dfs(node_t *now, value_t val) {
        int ptr = now->number_of_val() - 1;
        if (now->is_leaf) {
            while (ptr >= 0 && val < now->val[ptr]) {
                now->val[ptr + 1] = now->val[ptr];
                ptr--;
            }
            ptr++;
            now->val[ptr] = val;
            now->cnt++;
        } else {
            while (ptr >= 0 && val < now->val[ptr]) {
                ptr--;
            }
            ptr++;
            if (now->son[ptr]->number_of_son() == 4) {
                split(now, ptr);
                if (val > now->val[ptr]) {
                    ptr++;
                }
            }
            insert_dfs(now->son[ptr], val);
        }
    }

    void insert_impl(const value_t &val) {
        if (this->root == this->NIL) {
            this->root = create_node();
            this->root->cnt = 1;
            this->root->val[0] = val;
            this->root->is_leaf = true;
        } else {
            if (this->root->number_of_son() == 4) {
                node_t *s = create_node();
                s->son[0] = this->root;
                this->root = s;
                this->root->is_leaf = false;
                split(this->root, 0);
            }
            insert_dfs(this->root, val);
        }
    }

    inline value_t find_min(node_t *now) {
        while (!now->is_leaf) {
            now = now->son[0];
        }
        return now->val[0];
    }

    inline value_t find_max(node_t *now) {
        while (!now->is_leaf) {
            now = now->son[now->number_of_son() - 1];
        }
        return now->val[now->number_of_val() - 1];
    }

    // number of key word in now should be 2~3
    void erase_dfs(node_t *&_now, value_t val) {
        node_t *now = _now;
        int found = -1;
        for (int i = now->number_of_val() - 1; i >= 0; i--) {
            if (now->val[i] == val) {
                found = i;
            }
        }
        if (found != -1) {
            if (now->is_leaf) {
                if (found != now->number_of_val() - 1) {
                    this->val_shift_left(now, found + 1, now->number_of_val() - 1);
                }
                now->cnt--;
            } else {
                if (now->son[found]->number_of_val() >= 2) {
                    value_t next = find_max(now->son[found]);
                    now->val[found] = next;
                    erase_dfs(now->son[found], next);
                } else if (now->son[found + 1]->number_of_val() >= 2) {
                    value_t next = find_min(now->son[found + 1]);
                    now->val[found] = next;
                    erase_dfs(now->son[found + 1], next);
                } else {
                    node_t *l_part = now->son[found];
                    node_t *r_part = now->son[found + 1];

                    this->val_shift_left(now, found + 1, now->number_of_val() - 1);
                    this->son_shift_left(now, found + 2, now->number_of_son() - 1);
                    now->cnt--;
                    l_part->val[l_part->number_of_val()] = val;
                    l_part->val[l_part->number_of_val() + 1] = r_part->val[0];
                    l_part->son[l_part->number_of_son()] = r_part->son[0];
                    l_part->son[l_part->number_of_son() + 1] = r_part->son[1];
                    l_part->cnt += 2;
                    delete r_part;
                    erase_dfs(l_part, val);
                }
            }
        } else {
            int ptr = now->number_of_val() - 1;
            while (ptr >= 0 && val < now->val[ptr]) {
                ptr--;
            }
            ptr++;
            node_t *r_part = now->son[ptr];
            if (r_part->number_of_val() < 2) {
                if (ptr > 0 && now->son[ptr - 1]->number_of_val() >= 2) {
                    node_t *l_part = now->son[ptr - 1];
                    if (!l_part->is_leaf) {
                        node_t *sub_tree = l_part->son[l_part->number_of_son() - 1];
                        this->son_shift_right(r_part, 0, r_part->number_of_son());
                        r_part->son[0] = sub_tree;
                    }
                    value_t L = l_part->val[l_part->number_of_val() - 1];
                    l_part->cnt--;
                    value_t split_at = now->val[ptr - 1];
                    now->val[ptr - 1] = L;
                    this->val_shift_right(r_part, 0, r_part->number_of_val());
                    r_part->val[0] = split_at;
                    r_part->cnt++;
                } else if (ptr + 1 < now->number_of_son() && now->son[ptr + 1]->number_of_val() >= 2) {
                    node_t *l_part = now->son[ptr + 1];
                    if (!l_part->is_leaf) {
                        node_t *sub_tree = l_part->son[0];
                        this->son_shift_left(l_part, 1, l_part->number_of_son() - 1);
                        r_part->son[r_part->number_of_son()] = sub_tree;
                    }
                    value_t L = l_part->val[0];
                    this->val_shift_left(l_part, 1, l_part->number_of_val() - 1);
                    l_part->cnt--;
                    value_t split_at = now->val[ptr];
                    now->val[ptr] = L;
                    r_part->val[r_part->number_of_val()] = split_at;
                    r_part->cnt++;
                } else if (ptr > 0) {
                    value_t split_at = now->val[ptr - 1];
                    node_t *l_part = now->son[ptr - 1];
                    this->val_shift_left(now, ptr, now->number_of_val() - 1);
                    this->son_shift_left(now, ptr, now->number_of_son() - 1);

                    this->val_shift_right(r_part, 0, r_part->number_of_val() - 1, 2);
                    this->son_shift_right(r_part, 0, r_part->number_of_son() - 1, 2);
                    r_part->val[0] = l_part->val[0];
                    r_part->val[1] = split_at;

                    r_part->son[0] = l_part->son[0];
                    r_part->son[1] = l_part->son[1];
                    now->cnt--;
                    r_part->cnt += 2;
                    delete l_part;
                } else {
                    value_t split_at = now->val[ptr];
                    node_t *l_part = now->son[ptr + 1];
                    this->val_shift_left(now, ptr + 1, now->number_of_val() - 1);
                    this->son_shift_left(now, ptr + 1, now->number_of_son() - 1);

                    r_part->val[r_part->number_of_val()] = split_at;
                    r_part->val[r_part->number_of_val() + 1] = l_part->val[0];

                    r_part->son[r_part->number_of_son()] = l_part->son[0];
                    r_part->son[r_part->number_of_son() + 1] = l_part->son[1];
                    delete l_part;
                    now->son[ptr] = r_part;
                    now->cnt--;
                    r_part->cnt += 2;
                }
            }
            if (_now->number_of_son() == 1) {
                node_t *prev = _now;
                _now = _now->son[0];
                delete prev;
            }
            erase_dfs(r_part, val);
        }
    }

    void erase_impl(const value_t &val) {
        erase_dfs(root, val);
    }

    node_t *find_impl(const value_t &val) {
        if (root == this->NIL)return this->NIL;
        auto now = root;
        while (true) {
            int len = now->number_of_val();
            for (int i = 0; i < len; i++) {
                if (now->val[i] == val) {
                    return now;
                }
            }
            if (now->is_leaf) {
                break;
            } else {
                int ptr = now->number_of_val() - 1;
                while (ptr >= 0 && val < now->val[ptr]) {
                    ptr--;
                }
                ptr++;
                now = now->son[ptr];
            }
        }
        return this->NIL;
    }

    inline bool test_btree() {
        if (root == this->NIL)return true;
        bool ok = true;
        std::function<std::pair<int, int>(node_t *)> dfs = [&](node_t *now) -> std::pair<int, int> {
            if (now != this->root && (now->number_of_son() < 2 || now->number_of_son() > 4)) {
                ok = false;
            }
            for (int i = now->number_of_val() - 1; i >= 1; i--) {
                if (now->val[i] < now->val[i - 1]) {
                    ok = false;
                }
            }
            if (now->is_leaf)return {1, 1};
            else {
                int maxx = -1;
                int minn = 1e9;
                const int len = now->number_of_son();
                for (int i = 0; i < len; i++) {
                    auto[l, h]= dfs(now->son[i]);
                    maxx = std::max(maxx, h);
                    minn = std::min(minn, l);
                }
                if (minn != maxx) {
                    ok = false;
                }
                return std::make_pair(minn + 1, maxx + 1);
            }
        };
        dfs(root);
        return ok;
    }

    inline bool test_order() {
        if (root == this->NIL)return true;
        bool ok = true;
        std::function<std::pair<value_t, value_t>(node_t *)> dfs = [&](node_t *now) -> std::pair<value_t, value_t> {
            value_t maxx = std::numeric_limits<value_t>::min();
            value_t minn = std::numeric_limits<value_t>::max();
            for (int i = now->number_of_val() - 1; i >= 0; i--) {
                maxx = std::max(maxx, now->val[i]);
                minn = std::min(minn, now->val[i]);
            }
            if (!now->is_leaf) {
                const int N = now->number_of_son();
                for (int i = N - 1; i >= 0; i--) {
                    auto[l, h]= dfs(now->son[i]);
                    if (i != N - 1) {
                        if (h > now->val[i])ok = false;
                    }
                    if (i != 0) {
                        if (l < now->val[i - 1])ok = false;
                    }
                    maxx = std::max(maxx, h);
                    minn = std::min(minn, l);
                }
            }
            return {minn, maxx};
        };
        dfs(root);
        return ok;
    }

    bool test_impl() {
        return test_order() && test_btree();
    }

    int calc_height_impl() {
        if (root == this->NIL)return 0;
        int ret = 1;
        node_t *now = root;
        while (!now->is_leaf) {
            now = now->son[0];
            ret++;
        }
        return ret;
    }


    void walk_impl() {
        if (root == this->NIL) {
            std::cout << "NULL Tree" << std::endl;
        } else {
            static char sb[1000][1000];
            std::memset(sb, ' ', sizeof(sb));
            std::function<void(node_t *, int, int)> write = [&](node_t *now, int x, int y) {
                std::string str;
                std::stringstream ss;
                for (int i = 0; i < now->number_of_val(); i++) {
                    ss << now->val[i];
                    if (i + 1 != now->number_of_val())ss << ',';
                }
                ss << "(" << now->is_leaf << ")";
                ss >> str;
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
                write(now, x, y);
                if (now->is_leaf)return 1;
                sb[x + 1][y] = '|';
                sb[x + 1][y] = '|';
                for (int i = 0; i < 5; i++) {
                    sb[x + 1][y + 1 + i] = '-';
                }
                int used = 0;
                int prev = dfs(now->son[0], x + 1, y + 6) + 1;
                for (int o = 1; o < now->number_of_son(); o++) {
                    for (int i = 0; i < prev; i++) {
                        sb[x + 1 + 1 + i + used][y] = '|';
                    }
                    for (int i = 0; i < 5; i++) {
                        sb[x + 1 + used + prev][y + 1 + i] = '-';
                    }
                    used += prev;
                    prev = dfs(now->son[o], x + 1 + used, y + 6) + 1;
                }
                return used + prev;
            };
            dfs(root, 0, 0);
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

};

#endif