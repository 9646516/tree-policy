#ifndef TREE_POLICY_BENCH_HPP
#define TREE_POLICY_BENCH_HPP
#pragma comment(linker, "/STACK:512000000")

#include "lib.h"
#include <set>
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_set>
#include <mutex>

using namespace std::literals;
static const int TEST = 10;
static std::mutex mutex;

template<typename T>
static void test_tree(const char *sb, const std::vector<int> &V, const std::vector<int> &V2) {
    constexpr bool is_set = std::is_same_v<std::set<int>, std::decay_t<T>> ||
                            std::is_same_v<std::unordered_set<int>, std::decay_t<T>> ||
                            std::is_same_v<std::multiset<int>, std::decay_t<T>> ||
                            std::is_same_v<std::unordered_multiset<int>, std::decay_t<T>>;

    int64_t sum_insert = 0, sum_find = 0, sum_erase = 0;
    int h = 0;
    for (int o = 0; o < TEST; o++) {
        T *x = new T();
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (const int i: V) {
                x->insert(i);
            }
            auto end = std::chrono::high_resolution_clock::now();
            sum_insert += (end - start) / 1us;
        }

        {
            auto start = std::chrono::high_resolution_clock::now();
            for (const int i: V) {
                if constexpr(is_set) {
                    if (*x->find(i) != i) {
                        std::cout << "?" << std::endl;
                    }
                } else {
                    volatile auto res = x->find(i);
                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            sum_find += (end - start) / 1us;
        }
        if (o == 0) {
            if constexpr(!is_set) {
                h = x->calc_height();
            }
        }
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (const int &i: V2) {
                x->erase(i);
            }
            auto end = std::chrono::high_resolution_clock::now();
            sum_erase += (end - start) / 1us;
        }
        delete x;
    }
    std::lock_guard<std::mutex> guard(mutex);
    std::cout << "test" << ' ' << sb << std::endl;
    std::cout << "insert cost " << (double) sum_insert / TEST / 1000.0 << ' ' << "ms" << std::endl;
    std::cout << "find cost " << (double) sum_find / TEST / 1000.0 << ' ' << "ms" << std::endl;
    std::cout << "remove cost " << (double) sum_erase / TEST / 1000.0 << ' ' << "ms" << std::endl;
    std::cout << "tree height = " << h << std::endl;
    std::cout << std::endl;
}

void bench(const std::vector<int> &V, const std::vector<int> &V2) {
    std::cout.precision(3);
    std::cout << std::fixed;
    std::thread thread[]{
            std::thread(test_tree<std::multiset<int>>, "std::multiset", V, V2),
            std::thread(test_tree<BST<int>>, "bst", V, V2),
            std::thread(test_tree<Treap<int>>, "treap", V, V2),
            std::thread(test_tree<AVL<int>>, "avl", V, V2),
            std::thread(test_tree<Splay<int>>, "splay", V, V2),
            std::thread(test_tree<RBTree<int>>, "rb tree", V, V2),
            std::thread(test_tree<BTree<int>>, "2-3-4 tree", V, V2),
    };
    for (auto &i: thread) {
        i.join();
    }
}

#endif