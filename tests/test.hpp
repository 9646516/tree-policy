#ifndef TREE_POLICY_TEST_HPP
#define TREE_POLICY_TEST_HPP
#pragma comment(linker, "/STACK:512000000")

#include "lib.h"
#include <set>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

static std::mutex mutex;

template<typename tree_impl, typename node_impl, typename value_t>
static void test_tree(tree_trait<tree_impl, node_impl, value_t> *x, const char *sb, const std::vector<int> &V) {
    bool ok = true;
    auto F = [&]() {
        for (const int i: V) {
            x->insert(i);
            ok &= x->test();
            if (!ok) {
                std::cout << "F1" << std::endl;
                return;
            }
        }
        node_impl *node;
        for (const int i: V) {
            if constexpr(std::is_same_v<tree_impl, BTree<value_t>>) {
                node = x->find(i);
                bool found = false;
                for (int j = 0; j < node->number_of_val(); j++) {
                    if (node->val[j] == i) {
                        found = true;
                        break;
                    }
                }
                ok &= found;
            } else {
                ok &= (x->find(i)->val == i);
            }
            if (!ok) {
                std::cout << "F2 " << std::endl;
                return;
            }
        }
        for (const int i: V) {
            x->erase(i);
            ok &= x->test();
            if (!ok) {
                std::cout << "F3" << std::endl;
                return;
            }
        }
    };
    F();
    std::lock_guard<std::mutex> guard(mutex);
    std::cout << sb << " TEST " << (ok ? "PASS" : "FAILED") << std::endl;
    std::cout << std::endl;
}

template<typename T>
static void test_tree_wrap(const char *sb, const std::vector<int> &V) {
    T *x = new T();
    test_tree(x, sb, V);
    delete x;
}

void test(const std::vector<int> &V) {
    std::thread thread[]{
            std::thread(test_tree_wrap<BST<int>>, "bst", V),
            std::thread(test_tree_wrap<Treap<int>>, "treap", V),
            std::thread(test_tree_wrap<AVL<int>>, "avl", V),
            std::thread(test_tree_wrap<Splay<int>>, "splay", V),
            std::thread(test_tree_wrap<RBTree<int>>, "rb tree", V),
            std::thread(test_tree_wrap<BTree<int>>, "2-3-4 tree", V),
    };
    for (auto &i: thread) {
        i.join();
    }
}

#endif