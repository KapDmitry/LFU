#pragma once

#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

namespace caches {

template <typename T>
struct lfu_cache_value {
    T value;
    int frequency;
};

template <typename T, typename KeyT = int>
struct lfu_cache {
    std::unordered_map<KeyT, lfu_cache_value<T>> key_value_map;
    std::unordered_map<KeyT, std::list<T>> frequency_list_map;
    std::unordered_map<KeyT, typename std::list<T>::iterator> key_ptr_map;
    size_t capacity;
    int min_frequency;

   public:
    lfu_cache(size_t capacity) : capacity(capacity), min_frequency(0) {}

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page) {
        if (key_value_map.find(key) != key_value_map.end()) {
            update(key);
            return true;
        }

        put(key, slow_get_page(key));

        return false;
    }

   private:
    void invalidate_cache() {
        auto key_to_delete = frequency_list_map[min_frequency].back();
        frequency_list_map[min_frequency].pop_back();
        key_ptr_map.erase(key_to_delete);
        key_value_map.erase(key_to_delete);
    }

    void put(KeyT key, T value) {
        if (key_value_map.find(key) == key_value_map.end()) {
            if (key_value_map.size() < capacity) {
                insert_key(key, value);
            } else {
                invalidate_cache();
                insert_key(key, value);
            }
        } else {
            key_value_map[key].value = value;
            update(key);
        }
    }

    void insert_key(KeyT key, T value) {
        min_frequency = 1;
        key_value_map[key] = lfu_cache_value<T>{value, min_frequency};
        auto it = frequency_list_map[min_frequency].insert(
            frequency_list_map[min_frequency].begin(), key);
        key_ptr_map[key] = it;
    }

    void update(KeyT key) {
        auto freq = key_value_map[key].frequency++;
        auto it = key_ptr_map[key];
        frequency_list_map[freq].erase(it);
        if (frequency_list_map[freq].empty() && min_frequency == freq) {
            min_frequency = freq + 1;
        }
        ++freq;
        auto new_it = frequency_list_map[freq].insert(
            frequency_list_map[freq].begin(), key);
        key_ptr_map[key] = new_it;
    }
};

}  // namespace caches