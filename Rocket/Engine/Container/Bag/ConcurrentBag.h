#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <forward_list>
#include <cstdint>
#include <mutex>

namespace Rocket {
    template<typename Item>
    class ConcurrentBag {
    public:
        typedef std::forward_list<Item> List;
        typedef typename List::iterator Iterator;
        typedef typename List::const_iterator CIterator;
    private:
        std::recursive_mutex add_lock;
        List list;
        int32_t N;
    public:
        ConcurrentBag() : list(List()), N(0) {}
        bool empty() const{ return list.empty(); }
        int32_t size() const{ return N; }

        ConcurrentBag(const ConcurrentBag& list) = delete;
        ConcurrentBag& operator = (const ConcurrentBag& other) = delete;

        ConcurrentBag(ConcurrentBag&& bag) {
            std::scoped_lock guard{ bag.add_lock };
            list = bag.list;
            N = bag.N;
            bag.list.clear();
            bag.N = 0;
        }

        ConcurrentBag& operator = (ConcurrentBag&& other) {
            std::scoped_lock guard_this{ add_lock };
            std::scoped_lock guard_other{ other.add_lock };
            list = other.list;
            N = other.N;
            other.list.clear();
            other.N = 0;
        }

        void add(const Item &item) {
            std::scoped_lock guard{ add_lock };
            list.push_front(item);
            ++N;
        }

        void add(Item &&item) {
            std::scoped_lock guard{ add_lock };
            list.push_front(std::forward<Item>(item));
            ++N;
        }

        Iterator begin() { return list.begin(); }
        CIterator begin() const { return list.begin(); }

        Iterator end() { return list.end(); }
        CIterator end() const { return list.end(); }
    };
}
