#pragma once
#include "Containers/SymbolTable/ST.h"
#include "Containers/SymbolTable/BinarySearchST.h"

#include <vector>
#include <functional>

namespace Rocket {
    // TODO : define compare function
    template<typename Key, typename Value, typename Hash = std::hash<Key> >
    class SeparateChainingHashST : _implements_ ST<Key, Value> {
    public:
        SeparateChainingHashST(std::size_t init = INIT_CAPACITY) : M_(init) {
            for(int i = 0; i < M_; ++i) {
                st_.push_back(BinarySearchST<Key, Value>());
            }
        }

        virtual void put(const Key& key, const Value& value) final {
            if (N_ >= 10 * M_) resize(2 * M_);
            auto i = HashCode(key);
            if(!st_[i].contain(key)) N_++;
            st_[i].put(key, value);
        }

        virtual void remove(const Key& key) final {
            auto i = HashCode(key);
            if(st_[i].contain(key)) N_--;
            st_[i].remove(key);
            if (M_ > INIT_CAPACITY && N_ <= 2 * M_) resize(M_/2);
        }

        virtual Value get(const Key& key) const final {
            return st_[HashCode(key)].get(key);
        }

        virtual bool contain(const Key& key) const final {
            return st_[HashCode(key)].contain(key);
        }

        virtual std::vector<Key> keys() const final {
            std::vector<Key> key_vec;
            for(int i = 0; i < st_.size(); ++i) {
                for(auto key : st_[i].keys()) {
                    key_vec.push_back(key);
                }
            }
            return key_vec;
        }

        virtual std::vector<Value> values() const final {
            std::vector<Value> value_vec;
            for(int i = 0; i < st_.size(); ++i) {
                for(auto value : st_[i].values()) {
                    value_vec.push_back(value);
                }
            }
            return value_vec;
        }

        void resize(std::size_t size) {
            SeparateChainingHashST<Key, Value> temp(size);
            for(int i = 0; i < st_.size(); ++i) {
                for(auto key : st_[i].keys()) {
                    temp.put(key, st_[i].get(key));
                }
            }
            M_ = temp.M_;
            N_ = temp.N_;
            st_ = std::move(temp.st_);
        }

        virtual inline bool empty() const final { return size() == 0; }
        virtual inline std::size_t size() const final { return N_; }
    private:
        const std::size_t HashTextBook(const Key& key) const {
            return (hash_function_(key) & 0x7fffffff) % M_;
        }

        const std::size_t HashCode(const Key& key) const {
            auto h = hash_function_(key);
            h ^= (h >> 20) ^ (h >> 12) ^ (h >> 7) ^ (h >> 4);
            return h & (M_-1);
        }

    private:
        static const std::size_t INIT_CAPACITY = 4;
        std::size_t N_ = 0;
        std::size_t M_ = 0;
        // Could use SequentialSearchST instead,
        // but BinarySearchST will be faster
        std::vector<BinarySearchST<Key, Value> > st_ = {};
        Hash hash_function_ = Hash();
    };
}