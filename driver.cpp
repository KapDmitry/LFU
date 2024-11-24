#include <iostream>
#include "caches/lfu.hpp"
#include "caches/ideal.hpp"
#include <cassert>

int slow_get_page_int(int key) { return key; }

int main() {
    bool flag = false;

    #if MODE == 2
        flag = true;
    #endif

    int hits = 0;
    int n;
    size_t m;
 
    std::cin >> m >> n;
    assert(std::cin.good());
    caches::lfu_cache<int> c{m};
    caches::ideal_cache<int> ic{m};

    for (int i = 0; i < n; ++i) {
        int q;
        std::cin >> q;
        assert(std::cin.good());

        if (flag) {
            ic.update_history(q);
        }

        if (c.lookup_update(q, slow_get_page_int))
            hits += 1;
    }

    std::cout << hits << std::endl;

    if (flag) {
        std::cout << ic.test_cache(slow_get_page_int) << "\n";
    }
}