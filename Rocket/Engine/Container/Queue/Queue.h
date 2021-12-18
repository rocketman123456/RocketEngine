#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <forward_list>
#include <cstdint>
#include <exception>
#include <stdexcept>

namespace Rocket {
    template<typename Item>
    class Queue {
    public:
        typedef std::forward_list<Item> List;
        typedef typename List::iterator Iterator;
        typedef typename List::const_iterator CIterator;
    private:
        List list;
        Iterator last;
        int N;
    public:
        Queue() : list(List()), last(list.before_begin()), N(0) {}

        bool empty() const { return list.empty(); }
        int size() const { return N; }

        Queue(const Queue& list) = delete;
        Queue& operator = (const Queue& other) = delete;

        Queue(Queue&& queue) {
            list = queue.list;
            queue.list.clear();
        }

        Queue& operator = (Queue&& other) {
            list = other.list;
            other.list.clear();
        }

        void enqueue(const Item &item) {
            last = list.insert_after(last, item);
            ++N;
        }

        void enqueue(Item &&item) {
            last=list.insert_after(last, std::forward<Item>(item));
            ++N;
        }

        Item dequeue() {
            if (empty())
                throw std::underflow_error("Stack underflow");
            Item item = list.front();
            list.pop_front();
            --N;
            if (empty())
                last = list.before_begin();
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
