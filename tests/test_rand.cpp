#include "test.hpp"

#define RANDOM_SEED

int main() {
    const int N = 5e3;
    std::cout << "test rand" << std::endl;
    std::cout << "N = " << N << std::endl;
#ifdef RANDOM_SEED
    const int seed = (int) std::random_device{}();
#else
    const int seed = 444;
#endif
    std::vector<int> V;
    std::mt19937 rng(seed);
    V.reserve(N);
    for (int i = 0; i < N; i++) {
        V.push_back((int) rng());
    }
    test(V);
    return 0;
}