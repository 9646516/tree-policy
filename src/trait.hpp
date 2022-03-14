#ifndef TREE_POLICY_TRAIT_HPP
#define TREE_POLICY_TRAIT_HPP

#include <compare>
#include <iostream>
#include <functional>


template<typename tree_t, typename node_t, typename value_t>
struct tree_trait {
    node_t *NIL;

    void insert(const value_t &val) {
        static_cast<tree_t *>(this)->insert_impl(val);
    }

    void erase(const value_t &val) {
        static_cast<tree_t *>(this)->erase_impl(val);
    }

    void walk() {
        static_cast<tree_t *>(this)->walk_impl();
    }

    node_t *find(const value_t &val) {
        return static_cast<tree_t *>(this)->find_impl(val);
    }

    int calc_height() {
        return static_cast<tree_t *>(this)->calc_height_impl();
    }

    bool test() {
        return static_cast<tree_t *>(this)->test_impl();
    }
};

enum class COLOR : uint8_t {
    RED = 1,
    BLACK = 2
};
#endif