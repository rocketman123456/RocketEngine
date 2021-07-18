#pragma once
#include <exception>

namespace Rocket {
    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename T>
    class DoubleList {
        struct Node {
            Node* next = nullptr;
            Node* previous = nullptr;
            T data;
        };
        public:
        explicit DoubleList() {
            first_ = nullptr;
            last_ = nullptr;
        }
        DoubleList(const DoubleList& list) {
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
                temp->previous = last_;
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
        DoubleList(DoubleList&& list) {
            first_ = list.first_;
            last_ = list.last_;
            list.first_ = nullptr;
            list.first_last_ = nullptr;
        }
        virtual ~DoubleList() {
            while(first_) {
                auto temp = first_;
                first_ = first_->next;
                delete temp;
            }
            first_ = last_ = nullptr;
        }

        DoubleList* operator & () { return this; }
        const DoubleList* operator & () const { return this; }

        // Copy
        DoubleList& operator = (const DoubleList& other) {
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
                temp->previous = last_;
                temp->next = nullptr;
                last_->next = temp;
                if(nullptr == first_)
                    first_ = temp;
                last_ = temp;
                temp_first = temp_first->next;
            }
            return *this;
        }
        // Move
        DoubleList& operator = (DoubleList&& other) {
            first_ = other.first_;
            last_ = other.last_;
            other.first_ = nullptr;
            other.first_last_ = nullptr;
            return *this;
        }

        void InsertFront(const T& data) {
            auto temp = new Node;
            if(!last_) {
                last_ = temp;
            }
            temp->data = data;
            temp->next = first_;
            temp->previous = nullptr;
            if(first_) {
                first_->previous = temp;
            }
            first_ = temp;
        }
        void RemoveFront() {
            if(first_) {
                auto temp = first_;
                first_ = first_->next;
                delete temp;
                if(first_ == nullptr) {
                    first_ = last_ = nullptr;
                }
                else {
                    first_->previous = nullptr;
                }
            }
            else {
                throw std::out_of_range("Remove EmptyDouble List");
            }
        }

        void InsertBack(const T& data) {
            auto temp = new Node;
            if(!first_) {
                first_ = temp;
            }
            temp->data = data;
            temp->next = nullptr;
            temp->previous = last_;
            if(last_) {
                last_->next = temp;
            }
            last_ = temp;
        }
        void RemoveBack() {
            if(last_) {
                auto temp = last_;
                last_ = last_->previous;
                delete temp;
                if(last_ == nullptr) {
                    first_ = last_ = nullptr;
                }
                else {
                    last_->next = nullptr;
                }
            }
            else {
                throw std::out_of_range("Remove EmptyDouble List");
            }
        }

        bool IsEmpty() { return !first_; }
        T Front() { if(!first_) std::out_of_range("Empty Double List"); return first_->data; }
        T Last() { if(!last_) std::out_of_range("Empty Double List"); return last_->data; }

    private:
        Node* first_;
        Node* last_;
    };
}
