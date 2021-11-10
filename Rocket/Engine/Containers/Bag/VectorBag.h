#pragma once
#include <vector>
#include <cstdint>

namespace Rocket {
    template<typename Item>
    class VectorBag {
    public:
        typedef std::vector<Item> Vector;
        typedef typename Vector::iterator Iterator;
        typedef typename Vector::const_iterator CIterator;
    private:
        Vector vector;
    public:
        VectorBag() : vector(Vector()) {}
        bool empty() const{ return vector.empty(); }
        int32_t size() const{ return vector.size(); }

        VectorBag(const VectorBag& bag) = delete;
        VectorBag& operator = (const VectorBag& other) = delete;

        VectorBag(VectorBag&& vector) {
            vector = bag.vector;
            bag.vector.clear();
        }

        VectorBag& operator = (VectorBag&& other) {
            vector = other.vector;
            other.vector.clear();
        }

        void add(const Item &item) { vector.push_back(item); }
        void add(Item &&item) { vector.push_back(std::forward<Item>(item)); }

        Iterator begin() { return vector.begin(); }
        CIterator begin() const { return vector.begin(); }

        Iterator end() { return vector.end(); }
        CIterator end() const { return vector.end(); }
    };
}
