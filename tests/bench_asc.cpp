#include "bench.hpp"

int main() {
    const int N = 1e6;
    std::cout << "bench asc" << std::endl;
    std::cout << "N = " << N << std::endl;
    std::vector<int> V;
    V.reserve(N);
    for (int i = 0; i < N; i++) {
        V.push_back(i);
    }
    bench(V, V);
    return 0;
}