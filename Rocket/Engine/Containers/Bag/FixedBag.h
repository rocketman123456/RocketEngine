#pragma once
#include <forward_list>
#include <cstdint>
#include <exception>
#include <stdexcept>

namespace Rocket {
    template<typename Item, int32_t _N_>
    class FixedBag
    {
    public:
        typedef std::forward_list<Item> List;
        typedef typename List::iterator Iterator;
        typedef typename List::const_iterator CIterator;
    private:
        List list;
        int32_t N;
    public:
        FixedBag() : list(List()), N(0) {}
        bool empty() const{ return list.empty(); }
        int32_t size() const{ return N; }

        FixedBag(const FixedBag& list) = delete;
        FixedBag& operator = (const FixedBag& other) = delete;

        FixedBag(FixedBag&& bag) {
            list = bag.list;
            N = bag.N;
            bag.list.clear();
            bag.N = 0;
        }

        FixedBag& operator = (FixedBag&& other) {
            list = other.list;
            N = other.N;
            other.list.clear();
            other.N = 0;
        }

        void add(const Item &item) {
            if(N >= _N_) 
                throw std::overflow_error("FixedBag overflow");
            list.push_front(item);
            ++N;
        }

        void add(Item &&item) {
            if(N >= _N_) 
                throw std::overflow_error("FixedBag overflow");
            list.push_front(std::forward<Item>(item));
            ++N;
        }

        Iterator begin() { return list.begin(); }
        CIterator begin() const { return list.begin(); }

        Iterator end() { return list.end(); }
        CIterator end() const { return list.end(); }
    };
}
