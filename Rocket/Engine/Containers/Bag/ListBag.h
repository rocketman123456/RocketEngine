#pragma once
#include "Pattern/Iterator.h"

#include <exception>

namespace Rocket {
    template<typename T> class ListBag;
    template<typename T>
    class ListBagIterator : implements Iterator<T> {
    public:
        explicit ListBagIterator(ListBag<T>* bag) : bag_(bag), current_(bag->first_) {}
        virtual ~ListBagIterator() = default;
        virtual bool HasNext() final { return current_ == nullptr; }
        virtual T Next() final { T temp = current_->data; current_ = current_->next; return temp; }
    private:
        ListBag<T>* bag_ = nullptr;
        typename ListBag<T>::Node* current_ = nullptr;
    };

    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename T>
    class ListBag : implements Iterable<T, ListBagIterator<T>> {
        friend class ListBagIterator<T>;
        struct Node {
            Node* next = nullptr;
            T data;
        };
    public:
        explicit ListBag() : first_(nullptr), last_(nullptr) {}
        ListBag(const ListBag& list) {
            // delete exist data first
            first_ = last_ = nullptr;
            // skip empty list
            if(!list.first_)
                return;
            // deep copy list data
            auto temp_first = list.first_;
            while(temp_first) {
                auto temp = new Node;
                temp->data = temp_first->data;
                temp->next = nullptr;
                if(nullptr == last_)
                    last_ = temp;
                else {
                    last_->next = temp;
                    last_ = temp;
                }
                if(nullptr == first_)
                    first_ = temp;
                temp_first = temp_first->next;
            }
        }
        ListBag(ListBag&& list) {
            first_ = list.first_;
            last_ = list.last_;
            list.first_ = nullptr;
            list.first_last_ = nullptr;
        }
        virtual ~ListBag() {
            while(first_) {
                auto temp = first_;
                first_ = first_->next;
                delete temp;
            }
            first_ = last_ = nullptr;
        }

        virtual ListBagIterator<T> GetIterator() final {
            return ListBagIterator<T>(this);
        }

        ListBag* operator & () { return this; }
        const ListBag* operator & () const { return this; }

        // Copy
        ListBag& operator = (const ListBag& other) {
            // delete exist data first
            while(first_) {
                auto temp = first_;
                first_ = first_->next;
                delete temp;
            }
            first_ = last_ = nullptr;
            // skip empty list
            if(!other.first_)
                return *this;
            // deep copy list data
            auto temp_first = other.first_;
            while(temp_first) {
                auto temp = new Node;
                temp->data = temp_first->data;
                temp->next = nullptr;
                if(nullptr == last_)
                    last_ = temp;
                else {
                    last_->next = temp;
                    last_ = temp;
                }
                if(nullptr == first_)
                    first_ = temp;
                temp_first = temp_first->next;
            }
            return *this;
        }
        // Move
        ListBag& operator = (ListBag&& other) {
            first_ = other.first_;
            last_ = other.last_;
            other.first_ = nullptr;
            other.first_last_ = nullptr;
            return *this;
        }

        void Add(const T& data) {
            auto temp = new Node;
            if(!first_) {
                first_ = temp;
            }
            if(last_) {
                last_->next = temp;
            }
            temp->data = data;
            temp->next = nullptr;
            last_ = temp;
            size_++;
        }

        inline int32_t CurrentSize() const { return size_; }
        inline bool IsEmpty() const { return !first_; }
        inline T Front() { if(!first_) throw std::out_of_range("Get Data From Empty List"); return first_->data; }
        inline T Last() { if(!last_) throw std::out_of_range("Get Data From Empty List"); return last_->data; }

    private:
        Node* first_ = nullptr;
        Node* last_ = nullptr;
        int32_t size_ = 0;
    };
}
