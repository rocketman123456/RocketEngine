#pragma once
#include <forward_list>
#include <cstdint>
#include <exception>
#include <stdexcept>

// LIFO
namespace Rocket {
    template<typename Item>
    class ListStack {
    public:
        typedef std::forward_list<Item> List;
        typedef typename List::iterator Iterator;
        typedef typename List::const_iterator CIterator;
    private:
        List list;
        int N;
    public:
        ListStack() : list(List()), N(0) {}
        bool empty() const { return list.empty(); }
        int size() const { return N; }
        void push(const Item& item) { list.push_front(item); ++N; }
        void push(Item&& item) { list.push_front(std::forward<Item>(item)); ++N; }

        ListStack(const ListStack& list) = delete;
        ListStack& operator = (const ListStack& other) = delete;

        ListStack(ListStack&& stack) {
            list = stack.list;
            stack.list.clear();
        }

        ListStack& operator = (ListStack&& other) {
            list = other.list;
            other.list.clear();
        }

        Item pop() {
            if (empty())
                throw std::underflow_error("Stack underflow");
            Item item = list.front();
            list.pop_front();
            --N;
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
