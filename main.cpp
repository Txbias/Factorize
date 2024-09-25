#include <chrono>
#include <iostream>

#include "factorize.h"
#include "big_int.h"


int main() {
    const auto start = std::chrono::high_resolution_clock::now();

    runFactorization(BigInt("4175854084876627201"));

    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << elapsed.count() << " ms" << std::endl;
}
