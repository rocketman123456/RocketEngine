#pragma once
#include <forward_list>
#include <cstdint>
#include <exception>
#include <stdexcept>

// LIFO
namespace Rocket {
    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename Item, int32_t _N_>
    class FixedStack {
    public:
        typedef std::forward_list<Item> List;
        typedef typename List::iterator Iterator;
        typedef typename List::const_iterator CIterator;
    private:
        List list;
        int N;
    public:
        FixedStack() : list(List()), N(0) {}

        bool empty() const { return list.empty(); }
        int size() const { return N; }
        void push(const Item& item) { 
            if(N >= _N_) 
                throw std::overflow_error("FixedStack overflow");
            list.push_front(item); 
            ++N; 
        }
        void push(Item&& item) { 
            if(N >= _N_) 
                throw std::overflow_error("FixedStack overflow");
            list.push_front(std::forward<Item>(item)); 
            ++N; 
        }

        FixedStack(const FixedStack& list) = delete;
        FixedStack& operator = (const FixedStack& other) = delete;

        FixedStack(FixedStack&& stack) {
            list = stack.list;
            stack.list.clear();
        }

        FixedStack& operator = (FixedStack&& other) {
            list = other.list;
            other.list.clear();
        }

        Item pop() {
            if (empty())
                throw std::underflow_error("Stack underflow");
            Item item = list.front();
            list.pop_front();
            return item;
        }

        Item peek() const {
            if (empty())
                throw std::underflow_error("Stack underflow");
            return list.front();
        }

        Iterator begin() { return list.begin(); }
        CIterator begin() const { return list.begin(); }

        Iterator end() { return list.end(); }
        CIterator end() const { return list.end(); }
    };
}
