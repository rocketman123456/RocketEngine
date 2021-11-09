#pragma once
#include <vector>
#include <cstdint>
#include <exception>
#include <stdexcept>

// LIFO
namespace Rocket {
    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename Item>
    class Stack {
    public:
        typedef std::vector<Item> Vector;
        typedef typename Vector::iterator Iterator;
        typedef typename Vector::const_iterator CIterator;
    private:
        Vector vector;
    public:
        Stack() : vector(Vector()) {}

        bool empty() const { return vector.empty(); }
        int size() const { return vector.size(); }
        void push(const Item &item) { vector.push_back(item); }
        void push(Item &&item) { vector.push_back(std::forward<Item>(item)); }

        Stack(const Stack& list) = delete;
        Stack& operator = (const Stack& other) = delete;

        Stack(Stack&& stack) {
            vector = stack.vector;
            stack.vector.clear();
        }

        Stack& operator = (Stack&& other) {
            vector = other.vector;
            other.vector.clear();
        }

        Item pop() {
            if (empty())
                throw std::underflow_error("Stack underflow");
            Item item = vector.back();
            vector.erase(vector.end()-1);
            return item;
        }

        Item peek() const {
            if (empty())
                throw std::underflow_error("Stack underflow");
            return vector.back();
        }

        Iterator begin() { return vector.begin(); }
        CIterator begin() const { return vector.begin(); }

        Iterator end() { return vector.end(); }
        CIterator end() const { return vector.end(); }
    };
}
