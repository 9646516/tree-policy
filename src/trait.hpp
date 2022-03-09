#ifndef TREE_POLICY_TRAIT_HPP
#define TREE_POLICY_TRAIT_HPP

#include <compare>
#include <iostream>
#include <functional>

template<typename node_t, typename value_t>
struct basic_node_trait {
    node_t *L, *R, *fa;
    value_t val;
};

template<typename tree_t, typename node_t, typename value_t>
struct basic_tree_trait {
    void insert(const value_t &val) {
        static_cast<tree_t *>(this)->insert_impl(val);
    }

    void erase(const value_t &val) {
        static_cast<tree_t *>(this)->erase_impl(val);
    }

    node_t *find(const value_t &val) {
        return static_cast<tree_t *>(this)->find_impl(val);
    }

    void walk() {
        return static_cast<tree_t *>(this)->walk_impl();
    }
};

#endif