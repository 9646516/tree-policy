#include "bench.hpp"

int main() {
    const int N = 1e6;
    std::vector<int> V;
    V.reserve(N);
    for (int i = 0; i < N; i++) {
        V.push_back(i);
    }
    bench(V);
    return 0;
}