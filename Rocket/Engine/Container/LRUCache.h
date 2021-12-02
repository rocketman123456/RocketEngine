#pragma once
#include "Memory/MemoryDefine.h"

#include <deque>
#include <unordered_map>

namespace Rocket {
    template<typename S, typename T>
    class LRUCache {
    public:
        explicit LRUCache(std::size_t c) : capacity(c) {}

        T get(const S& key) {
            auto found = resource_map.find(key);
            if(found != resource_map.end()) {
                std::pair<S, T> p = *(found->second);
                history.erase(resource_map[key]);
                history.push_front(p);
                (found->second) = history.begin();
                return p.second;
            } else {
                return T();
            }
        }

        void put(const S& key, const T& value) {
            auto found = resource_map.find(key);
            if(found != resource_map.end()) {
                history.erase(found->second);
                history.push_front({key, value});
                (found->second) = history.begin();
            } else {
                if(history.size() >= capacity) {
                    auto last = history.back();
                    history.pop_back();
                    resource_map.erase(last.first);
                }
                history.push_front({key, value});
                resource_map.insert({key, history.begin()});
            }
        }
    private:
        //typename  Iter;
        using ResourceMap = std::unordered_map<S, typename std::deque<std::pair<S, T> >::iterator>;
        std::size_t capacity = 0;
        std::deque<std::pair<S, T> > history;
        ResourceMap resource_map;
    };
}