#pragma once

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

namespace caches {

const int MAX_IND = 2000000000;

template <typename T, typename KeyT = int>
struct ideal_cache {
    size_t capacity;
    std::unordered_map<KeyT, T> key_value;
    std::unordered_map<KeyT,std::vector<T>> history;
    std::vector<T> raw_history; 
    int cur_index;

   public:
    ideal_cache(size_t capacity) : capacity(capacity), cur_index(-1) {}

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page) {
        ++cur_index;
        if (key_value.find(key) != key_value.end()) {
            return true;
        }

        put(key, slow_get_page(key));
        return false;
    }

    void update_history(KeyT key) {
        raw_history.push_back(key);
        history[key].push_back(raw_history.size()-1);
    }

    template <typename F>
    int test_cache(F slow_get_page) {
        int hits = 0;
        for (size_t i = 0; i < raw_history.size(); ++i) {
            if (lookup_update(raw_history[i], slow_get_page)) {
                hits += 1;
            }
        }
        return hits;
    }


   private:
     void put(KeyT key, T value) {
        if (key_value.size() >= capacity) {
            invalidate_cache();
        }

        key_value[key] = value;

        return;
    }

    void invalidate_cache() {
        int last_ind = -1;
        KeyT key_to_delete;

        for (auto it = history.begin(); it != history.end(); ++it) {
            size_t len_cnt = 0;
            for (auto ind : it->second) {
                if (ind > cur_index && ind > last_ind) {
                    last_ind = ind;
                    key_to_delete = it->first;
                }
                if (len_cnt == it->second.size()-1) {
                    if (ind < cur_index) {
                        ind = MAX_IND;
                        key_to_delete = it->first;
                    }
                }
                ++len_cnt;
            }
        }

        key_value.erase(key_to_delete);
    }


    
};

}  // namespace caches