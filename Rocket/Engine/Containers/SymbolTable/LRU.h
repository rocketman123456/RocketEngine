#pragma once

#include <deque>
#include <unordered_map>

namespace Rocket {
    template<typename S, typename T>
    class LRU {
    public:
        explicit LRU(std::size_t c) : capacity(c) {}

        T get(const S& key) {
            if(umap.find(key) != umap.end()) {
                std::pair<S, T> p = *(umap[key]);
                dq.erase(umap[key]);
                dq.push_front(p);
                umap[key] = dq.begin();
                return p.second;
            } else {
                return T();
            }
        }

        void put(const S& key, const T& value) {
            if(umap.find(key) != umap.end()) {
                dq.erase(umap[key]);
                dq.push_front({key, value});
                umap[key] = dq.begin();
            } else {
                if(dq.size() >= capacity) {
                    auto last = dq.back();
                    dq.pop_back();
                    umap.erase(last.first);
                    dq.push_front({key, value});
                    umap.insert({key, dq.begin()});
                } else {
                    dq.push_front({key, value});
                    umap.insert({key, dq.begin()})
                }
            }
        }
    private:
        std::size_t capacity = 0;
        std::deque<std::pair<S, T>> dq;
        std::unordered_map<S, std::deque<std::pair<S, T>>::iterator> umap;
    };
}