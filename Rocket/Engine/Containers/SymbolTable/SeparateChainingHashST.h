#pragma once
#include "Containers/SymbolTable/ST.h"
#include "Containers/SymbolTable/BinarySearchST.h"

#include <vector>
#include <functional>

namespace Rocket {
    template<typename Key, typename Value, typename Hash = std::hash<Key>>
    class SeparateChainingHashST : _implements_ ST<S, T>, _implements_ ST_Full<S, T> {
    public:
        SeparateChainingHashST(std::size_t init, const Hash& hash_func) 
                : M_(init), hash_function_(hash_func) {
            st_.resize(M_);
            for(int i = 0; i < M_; ++i) {
                st_.push_back(BinarySearchST<Key, Value>());
            }
        }

        virtual void put(const Key& key, const Value& value) final;
        virtual void remove(const Key& key) final;
        virtual const Value get(const Key& key) const final;
        virtual bool contain(const Key& key) const final;
        virtual bool empty() const final { return size() == 0; }
        virtual std::size_t size() const final;
        virtual std::vector<Key> keys() const final;
        virtual std::vector<Value> values() const final;

        virtual Key min() const final;
        virtual Key max() const final;
        virtual Key ceiling(const Key& key) const final;
        virtual Key floor(const Key& key) const final;
    private:
        std::size_t HashTextBook(Key key) {
            return (hash_function_(key) & 0x7fffffff) % m;
        }

        std::size_t HashCode(Key key) {
            auto h = hash_function_(key);
            h ^= (h >>> 20) ^ (h >>> 12) ^ (h >>> 7) ^ (h >>> 4);
            return h & (m-1);
        }
    private:
        constexpr std::size_t INIT_CAPACITY = 4;
        std::size_t N_;
        std::size_t M_;
        std::vector<BinarySearchST<Key, Value>> st_;
        Hash hash_function_;
    };
}