#pragma once
#include "Containers/SymbolTable/ST.h"

#include <map>
#include <cstddef>
#include <functional>

namespace Rocket {
    // TODO : define compare function
    template<typename Key, typename Value, typename Hash = std::hash<Key> >
    class SymbolTable : _implements_ ST<Key, Value> {
    public:
        virtual void put(const Key& key, const Value& value) final { map_[key] = value; }
        virtual void remove(const Key& key) final { if(map_.find(key) != map_.end()) map_.erase(map_.find(key)); }
        virtual bool empty() const final { return map_.empty(); }
        virtual std::size_t size() const final { return map_.size(); }

        virtual Value get(const Key& key) const final {
            if(map_.find(key) != map_.end())
                return map_.at(key);
            else 
                assert(false && "Unable to get value");
        }

        virtual bool contain(const Key& key) const final {
            if(map_.find(key) != map_.end())
                return true;
            else
                return false;
        }

        virtual std::vector<Key> keys() const final { 
            std::vector<Key> key_vec;
            for(auto item : map_) {
                key_vec.push_back(item.first);
            }
            return key_vec;
        }

        virtual std::vector<Value> values() const final {
            std::vector<Value> value_vec;
            for(auto item : map_) {
                value_vec.push_back(item.second);
            }
            return value_vec;
        }

        // virtual Key min() const final;
        // virtual Key max() const final;
        // virtual Key ceiling(const Key& key) const final;
        // virtual Key floor(const Key& key) const final;
    private:
        std::map<Key, Value> map_;
        Hash hash_function = Hash();
    };
}
