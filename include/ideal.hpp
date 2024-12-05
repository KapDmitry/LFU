#pragma once

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

namespace caches {

template <typename T, typename KeyT = int>
struct ideal_cache {
    size_t capacity;
    std::unordered_map<KeyT, T> key_value;
    std::vector<T> raw_history; 
    std::unordered_map<KeyT,std::list<int>> history;

   public:

    ideal_cache(size_t capacity) : capacity(capacity) {}

    void add_history(KeyT key) {
        raw_history.push_back(key);
    }

    template <typename F>
    int test_cache(F slow_get_page) {
        map();

        int hits = 0;

        for (size_t i = 0; i < raw_history.size(); ++i) {
            if (lookup_update(raw_history[i], slow_get_page)) {
                hits += 1;
            }
            update_history(raw_history[i]);
        }

        return hits;
    }


   private:
    
    void map() {
        for (size_t i = 0; i < raw_history.size(); ++i) {
            history[raw_history[i]].push_back(i);
        }
    }

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page) {
        if (key_value.find(key) != key_value.end()) {
            return true;
        }

        put(key, slow_get_page(key));

        return false;
    }


    void put(KeyT key, T val) {
        if (history[key].size() == 1) {
            return;
        }

        if (key_value.size() >= capacity) {
            invalidate_cache();
        }

        key_value[key] = val;
    }

    void invalidate_cache() {
        auto key_to_delete = find_key_to_delete();
        key_value.erase(key_to_delete);
    }

    KeyT find_key_to_delete() {
        KeyT key_to_delete; 
        int max_index = -1;         

        for (const auto& [key, value] : key_value) {
            if (history[key].empty()) {
                return key;
            } else {
                int next_index = history[key].front();

                if (next_index > max_index) {
                    max_index = next_index;
                    key_to_delete = key;
                }
            }
        }

        return key_to_delete;
    }

    void update_history(KeyT key) {
        history[key].pop_front();
    }

};

}  // namespace caches