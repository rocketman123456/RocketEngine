#pragma once
#include "Containers/SymbolTable/ST.h"

#include <vector>
#include <memory>
#include <cstddef>
#include <cassert>

namespace Rocket {
    template<typename S, typename T>
    class BinarySearchST : _implements_ ST<S, T>, _implements_ ST_Full<S, T> {
    public:
        virtual void put(const S& key, const T& value) final;
        virtual void remove(const S& key) final;
        virtual const T get(const S& key) const final;
        virtual bool contain(const S& key) const final;
        virtual bool empty() const final;
        virtual std::size_t size() const final;
        virtual std::vector<S> keys() const final;
        virtual std::vector<T> values() const final;
        virtual S min() const final;
        virtual S max() const final;
        virtual S ceiling(const S& key) const final;
        virtual S floor(const S& key) const final;

        std::size_t rank(const S& key) const;
    private:
        std::vector<S> keys_;
        std::vector<T> values_;
    };

    template<typename S, typename T>
    std::size_t BinarySearchST<S,T>::rank(const S& key) const {
        if(this->size() == 0) 
            return 0;
        std::size_t lo = 0;
        std::size_t hi = this->size() - 1;
        std::size_t mid;
        while(lo < hi) {
            mid = lo + (hi - lo) / 2;
            if(keys_[mid] < key) 
                lo = mid + 1;
            else 
                hi = mid;
        }
        if(keys_[lo] < key)
            return lo + 1;
        else 
            return lo;
    }

    template<typename S, typename T>
    void BinarySearchST<S,T>::put(const S& key, const T& value) {
        std::size_t pos = rank(key);
        if(pos < this->size() && keys_[pos] == key) { 
            values_[pos] = value;
            return;
        }
        // First Make Vector Bigger
        keys_.push_back(key);
        values_.push_back(value);
        for(int i = this->size() - 1; i > pos; --i) {
            keys_[i] = keys_[i - 1];
            values_[i] = values_[i - 1];
        }
        keys_[pos] = key;
        values_[pos] = value;
    }

    template<typename S, typename T>
    void BinarySearchST<S,T>::remove(const S& key) {
        std::size_t pos = rank(key);
        if(pos < this->size() && keys_[pos] == key) {
            keys_.erase(keys_.begin() + pos);
            values_.erase(values_.begin() + pos);
        }
    }

    template<typename S, typename T>
    const T BinarySearchST<S,T>::get(const S& key) const {
        std::size_t pos = rank(key);
        if(pos < this->size() && keys_[pos] == key) { 
            return values_[pos];
        }
        return T();
    }

    template<typename S, typename T>
    bool BinarySearchST<S,T>::contain(const S& key) const {
        std::size_t pos = rank(key);
        if(pos < this->size() && keys_[pos] == key) { 
            return true;
        }
        return false;
    }

    template<typename S, typename T>
    bool BinarySearchST<S,T>::empty() const {
        return keys_.empty();
    }

    template<typename S, typename T>
    std::size_t BinarySearchST<S,T>::size() const {
        return keys_.size();
    }

    template<typename S, typename T>
    std::vector<S> BinarySearchST<S,T>::keys() const {
        return keys_;
    }

    template<typename S, typename T>
    std::vector<T> BinarySearchST<S,T>::values() const {
        return values_;
    }

    template<typename S, typename T>
    S BinarySearchST<S,T>::min() const {
        assert(keys_.size() > 0);
        return *(keys_.begin());
    }

    template<typename S, typename T>
    S BinarySearchST<S,T>::max() const {
        assert(keys_.size() > 0);
        return *(keys_.end()-1);
    }

    template<typename S, typename T>
    S BinarySearchST<S,T>::ceiling(const S& key) const {
        assert(keys_.size() > 0);
        auto pos = rank(key);
        assert(pos < this->size() && "argument to ceiling() is too large");
        return keys_[pos];
    }

    template<typename S, typename T>
    S BinarySearchST<S,T>::floor(const S& key) const {
        assert(keys_.size() > 0);
        auto pos = rank(key);
        if (pos < this->size() && key == keys_[pos]) return keys_[pos];
        assert(pos > 0 && "argument to floor() is too small");
        return keys_[pos-1];
    }
}