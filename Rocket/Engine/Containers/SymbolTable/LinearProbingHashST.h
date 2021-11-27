#pragma once
#include "Containers/SymbolTable/ST.h"

#include <vector>
#include <functional>

namespace Rocket {
    // TODO : define compare function
    // TODO : finish put and get function
    template<typename Key, typename Value>
    class LinearProbingHashST : _implements_ ST<Key, Value> {
    public:
        LinearProbingHashST(std::size_t capacity = INIT_CAPACITY) : M_(capacity) {
            keys_ = new Key[M_];
            values_ = new Value[M_];
        }

        virtual  ~LinearProbingHashST() {
            if(keys_) delete [] keys_;
            if(values_) delete [] values_;
        }

        virtual void put(const Key& key, const Value& value) final {
            // if (N_ >= M_/2) resize(2 * M_);
            // int i;
            // for (i = hash(key); keys_[i] != nullptr; i = (i + 1) % m) {
            //     if (keys_[i].equals(key)) {
            //         values_[i] = value;
            //         return;
            //     }
            // }
            // keys_[i] = key;
            // values_[i] = value;
            // N_++;
        }

        virtual void remove(const Key& key) final {
            // find position i of key
            // int i = hash(key);
            // while (!key.equals(keys[i])) {
            //     i = (i + 1) % m;
            // }

            // // delete key and associated value
            // keys[i] = null;
            // vals[i] = null;

            // // rehash all keys in same cluster
            // i = (i + 1) % m;
            // while (keys[i] != null) {
            //     // delete keys[i] an vals[i] and reinsert
            //     Key   keyToRehash = keys[i];
            //     Value valToRehash = vals[i];
            //     keys[i] = null;
            //     vals[i] = null;
            //     n--;
            //     put(keyToRehash, valToRehash);
            //     i = (i + 1) % m;
            // }

            // n--;

            // // halves size of array if it's 12.5% full or less
            // if (n > 0 && n <= m/8) resize(m/2);

            // assert(check());
        }

        virtual Value get(const Key& key) const final {
            for(int i = 0; i < size(); ++i) {
                if(key == keys_[i]) {
                    return values_[i];
                }
            }
            return Value();
        }

        virtual bool contain(const Key& key) const final {
            for(int i = 0; i < size(); ++i) {
                if(key == keys_[i]) {
                    return true;
                }
            }
            return false;
        }

        virtual bool empty() const final { return size() == 0; }
        virtual std::size_t size() const final { return N_; }

        virtual std::vector<Key> keys() const final {
            std::vector<Key> key_vec;
            for(int i = 0; i < size(); ++i) {
                key_vec.push_back(keys_[i]);
            }
            return key_vec;
        }

        virtual std::vector<Value> values() const final {
            std::vector<Value> value_vec;
            for(int i = 0; i < size(); ++i) {
                value_vec.push_back(values_[i]);
            }
            return value_vec;
        }
    private:
        void resize(std::size_t capacity) {
            Key* key = new Key[capacity];
            Value* value = new Value[capacity];
            for(int i = 0; i < size(); ++i) {
                key[i] = keys_[i];
                value[i] = values_[i];
            }
            delete [] keys_;
            delete [] values_;
            keys_ = key;
            values_ = value;
            M_ = capacity;
        }

        bool check() {
            // // check that hash table is at most 50% full
            // if (M_ < 2*N_) {
            //     //System.err.println("Hash table size m = " + m + "; array size n = " + n);
            //     return false;
            // }
            // // check that each key in table can be found by get()
            // for (int i = 0; i < M_; i++) {
            //     if (keys_[i] == nullptr) continue;
            //     else if (get(keys_[i]) != values_[i]) {
            //         //System.err.println("get[" + keys[i] + "] = " + get(keys[i]) + "; vals[i] = " + vals[i]);
            //         return false;
            //     }
            // }
            // return true;
        }

        const std::size_t HashCode(const Key& key) const {
            auto h = hash_function_(key);
            h ^= (h >> 20) ^ (h >> 12) ^ (h >> 7) ^ (h >> 4);
            return h & (M_-1);
        }
    private:
        static constexpr std::size_t INIT_CAPACITY = 4;
        std::size_t N_ = 0;
        std::size_t M_ = 0;
        Key* keys_ = nullptr;
        Value* values_ = nullptr;
    };
}
