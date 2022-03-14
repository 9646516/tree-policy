#include "bench.hpp"

#define RANDOM_SEED

int main() {
    const int N = 1e6;
    std::cout << "bench rand" << std::endl;
    std::cout << "N = " << N << std::endl;
#ifdef RANDOM_SEED
    const int seed = (int) std::random_device{}();
#else
    const int seed = 114514;
#endif
    std::vector<int> V, V2;
    std::mt19937 rng(seed);
    V.reserve(N);
    for (int i = 0; i < N; i++) {
        V.push_back(i);
    }
    V2.assign(V.begin(), V.end());
    std::shuffle(V.begin(), V.end(), rng);
    std::shuffle(V2.begin(), V2.end(), rng);
    bench(V, V2);
    return 0;
}