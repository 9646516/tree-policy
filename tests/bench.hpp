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
static void test_tree(const char *sb, const std::vector<int> &V) {
    int64_t sum_insert = 0, sum_find = 0, sum_erase = 0;

    for (int _ = 0; _ < TEST; _++) {
        T *x = new T();
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (auto &i: V) {
                x->insert(i);
            }
            auto end = std::chrono::high_resolution_clock::now();
            sum_insert += (end - start) / 1us;
        }
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (auto &i: V) {
                auto res = x->find(i);
                if constexpr(std::is_same_v<std::set<int>, std::decay_t<T>> ||
                             std::is_same_v<std::unordered_set<int>, std::decay_t<T>> ||
                             std::is_same_v<std::multiset<int>, std::decay_t<T>> ||
                             std::is_same_v<std::unordered_multiset<int>, std::decay_t<T>>
                        ) {
                    if (*res != i) {
                        std::cout << "bad" << std::endl;
                    }
                } else {
                    if (res->val != i) {
                        std::cout << "bad" << std::endl;
                    }
                }

            }
            auto end = std::chrono::high_resolution_clock::now();
            sum_find += (end - start) / 1us;
        }
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (auto &i: V) {
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
    std::cout << std::endl;
}

void bench(const std::vector<int> &V) {
    std::thread t0(test_tree<std::multiset<int>>, "std::multiset", V);
    std::thread t1(test_tree<BST<int>>, "bst", V);
    std::thread t2(test_tree<Treap<int>>, "treap", V);
    std::thread t3(test_tree<AVL<int>>, "avl", V);
    std::thread t4(test_tree<Splay<int>>, "splay", V);
    for (auto i: {&t0, &t1, &t2, &t3, &t4}) {
        i->join();
    }
}