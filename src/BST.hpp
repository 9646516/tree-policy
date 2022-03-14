#ifndef TREE_POLICY_BST_HPP
#define TREE_POLICY_BST_HPP

#include "basic_tree.hpp"

template<typename value_t>
struct BST_node : public basic_node<BST_node<value_t>, value_t> {
};

template<typename value_t>
struct BST : public basic_tree<BST<value_t>, BST_node<value_t>, value_t> {
    using node_t = BST_node<value_t>;

    void insert_impl(const value_t &val) {
        this->_insert_impl(val);
    }

    void erase_impl(const value_t &val) {
        this->_erase_impl(val);
    }

    node_t *find_impl(const value_t &val) {
        return this->_find_impl(val);
    }

    void walk_impl() {
        return this->_walk_impl();
    }

    bool test_impl() {
        return this->test_bst();
    }
};

#endif
