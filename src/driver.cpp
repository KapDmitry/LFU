#include <iostream>
#include <cassert>

#ifdef LFU_CACHE
#include "lfu.hpp"
#endif

#ifdef IDEAL_CACHE
#include "ideal.hpp"
#endif

int slow_get_page_int(int key) { return key; }

int main() {
    int n;
    size_t m;
 
    std::cin >> m >> n;
    assert(std::cin.good());

    #ifdef LFU_CACHE
        int hits = 0;
        caches::lfu_cache<int> c{m};
    #endif

    #ifdef IDEAL_CACHE
        caches::ideal_cache<int> ic{m};
    #endif

    for (int i = 0; i < n; ++i) {
        int q;
        std::cin >> q;
        assert(std::cin.good());

        #ifdef IDEAL_CACHE
            ic.add_history(q);
        #endif

        #ifdef LFU_CACHE
            if (c.lookup_update(q, slow_get_page_int))
                hits += 1;
        #endif
    }

    #ifdef LFU_CACHE
        std::cout << hits << std::endl;
    #endif

    #ifdef IDEAL_CACHE
        std::cout << ic.test_cache(slow_get_page_int) << "\n";
    #endif

    return 0;
}
