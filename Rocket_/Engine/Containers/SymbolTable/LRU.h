#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <deque>
#include <unordered_map>

namespace Rocket {
    template<typename S, typename T>
    class LRU {
    public:
        explicit LRU(std::size_t c) : capacity(c) {}

        T get(const S& key) {
            if(resource_map.find(key) != resource_map.end()) {
                std::pair<S, T> p = *(resource_map[key]);
                history.erase(resource_map[key]);
                history.push_front(p);
                resource_map[key] = history.begin();
                return p.second;
            } else {
                return T();
            }
        }

        void put(const S& key, const T& value) {
            if(resource_map.find(key) != resource_map.end()) {
                history.erase(resource_map[key]);
                history.push_front({key, value});
                resource_map[key] = history.begin();
            } else {
                if(history.size() >= capacity) {
                    auto last = history.back();
                    history.pop_back();
                    resource_map.erase(last.first);
                    history.push_front({key, value});
                    resource_map.insert({key, dq.begin()});
                } else {
                    history.push_front({key, value});
                    resource_map.insert({key, dq.begin()})
                }
            }
        }
    private:
        std::size_t capacity = 0;
        std::deque<std::pair<S, T>> history;
        std::unordered_map<S, std::deque<std::pair<S, T>>::iterator> resource_map;
    };
}