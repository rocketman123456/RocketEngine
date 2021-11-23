#pragma once
#include "Containers/SymbolTable/ST.h"

#include <list>
#include <vector>
#include <memory>

namespace Rocket {
    template<typename S, typename T>
    class SequentialSearchST : _implements_ ST<S, T> {
        struct Node {
            Node(const S& key, const T& value) :key(key), value(value) {}
            const S key;
            T value;
        };
    public:
        virtual void put(const S& key, const T& value) final;
        virtual void remove(const S& key) final;
        virtual const T get(const S& key) const final;
        virtual bool contain(const S& key) const final;
        virtual std::vector<S> keys() const final;
        virtual std::vector<T> values() const = 0;
        virtual S min() const final;
        virtual S max() const final;
        virtual S ceiling(const T& key) const final;
        virtual S floor(const T& key) const final;

        virtual inline bool empty() const final { return node_list_.empty(); }
        virtual inline std::size_t size() const final { return node_list_.size(); }
    private:
        using NodePtr = std::shared_ptr<Node>;
        std::list<NodePtr> node_list_;
    };

    template<typename S, typename T>
    const T SequentialSearchST<S,T>::get(const S& key) const {
        NodePtr result = nullptr;
        for(auto iter = node_list_.begin(); iter != node_list_.end(); ++iter) {
            if((*iter)->key == key) {
                result = *iter;
                break;
            }
        }

        if(!result) 
            return T();
        else 
            return result->value;
    }

    template<typename S, typename T>
    void SequentialSearchST<S,T>::put(const S& key, const T& value) {
        NodePtr result = nullptr;
        for(auto iter = node_list_.begin(); iter != node_list_.end(); ++iter) {
            if((*iter)->key == key) {
                result = *iter;
                break;
            }
        }

        if(!result) {
            node_list_.push_back(std::make_shared<Node>(key, val));
        } else {
            result->value = value;
        }
    }

    template<typename S, typename T>
    void SequentialSearchST<S,T>::remove(const S& key) {
        for(auto iter = node_list_.begin(); iter != node_list_.end(); ++iter) {
            if((*iter)->key == key) {
                node_list_.erase(iter);
                break;
            }
        }
    }

    template<typename S, typename T>
    bool SequentialSearchST<S,T>::contain(const S& key) const {
        bool found = false;
        for(auto iter = node_list_.begin(); iter != node_list_.end(); ++iter) {
            if((*iter)->key == key) {
                found = true;
                break;
            }
        }
        return found;
    }

    template<typename S, typename T>
    std::vector<S> SequentialSearchST<S,T>::keys() const {
        std::vector<S> res;
        for(auto iter = node_list_.begin(); iter != node_list_.end(); ++iter) {
            res.push_back((*iter)->key);
        }
        return res;
    }

    template<typename S, typename T>
    std::vector<T> SequentialSearchST<S,T>::values() const {
        std::vector<T> res;
        for(auto iter = node_list_.begin(); iter != node_list_.end(); ++iter) {
            res.push_back((*iter)->value);
        }
        return res;
    }

    template<typename S, typename T>
    S SequentialSearchST<S,T>::min() const {
        
    }

    template<typename S, typename T>
    S SequentialSearchST<S,T>::max() const {
        
    }

    template<typename S, typename T>
    S SequentialSearchST<S,T>::ceiling(const T& key) const {
        
    }

    template<typename S, typename T>
    S SequentialSearchST<S,T>::floor(const T& key) const {
        
    }
}
