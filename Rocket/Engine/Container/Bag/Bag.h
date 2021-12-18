#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <forward_list>
#include <cstdint>

namespace Rocket {
    template<typename Item>
    class Bag {
    public:
        typedef std::forward_list<Item> List;
        typedef typename List::iterator Iterator;
        typedef typename List::const_iterator CIterator;
    private:
        List list;
        int32_t N;
    public:
        Bag() : list(List()), N(0) {}
        bool empty() const{ return list.empty(); }
        int32_t size() const{ return N; }

        Bag(const Bag& list) = delete;
        Bag& operator = (const Bag& other) = delete;

        Bag(Bag&& bag) {
            list = bag.list;
            N = bag.N;
            bag.list.clear();
            bag.N = 0;
        }

        Bag& operator = (Bag&& other) {
            list = other.list;
            N = other.N;
            other.list.clear();
            other.N = 0;
        }

        void add(const Item &item) {
            list.push_front(item);
            ++N;
        }

        void add(Item &&item) {
            list.push_front(std::forward<Item>(item));
            ++N;
        }

        Iterator begin() { return list.begin(); }
        CIterator begin() const { return list.begin(); }

        Iterator end() { return list.end(); }
        CIterator end() const { return list.end(); }
    };
}
