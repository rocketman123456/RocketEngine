#pragma once
#include "Containers/SymbolTable/ST.h"

namespace Rocket {
    template<typename Key, typename Value>
    class SymbolTable : _implements_ ST<Key, Value> {
    public:
        virtual void put(const Key& key, const Value& value) final;
        virtual void remove(const Key& key) final;
        virtual const Value get(const Key& key) const final;
        virtual bool contain(const Key& key) const final;
        virtual bool empty() const final;
        virtual std::size_t size() const final;
        virtual std::vector<Key> keys() const final;
        virtual std::vector<Value> values() const final;
        virtual Key min() const final;
        virtual Key max() const final;
        virtual Key ceiling(const Key& key) const final;
        virtual Key floor(const Key& key) const final;
    };
}
