#pragma once
#include <exception>
#include <stdexcept>

namespace Rocket {
    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename T>
    class List {
        struct Node {
            Node* next = nullptr;
            T data;
        };
    public:
        explicit List() : first_(nullptr), last_(nullptr) {}
        List(const List& list) {
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
        List(List&& list) {
            first_ = list.first_;
            last_ = list.last_;
            list.first_ = nullptr;
            list.first_last_ = nullptr;
        }
        virtual ~List() {
            while(first_) {
                auto temp = first_;
                first_ = first_->next;
                delete temp;
            }
            first_ = last_ = nullptr;
        }

        List* operator & () { return this; }
        const List* operator & () const { return this; }

        // Copy
        List& operator = (const List& other) {
            if (this == &other)
                return *this;
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
        List& operator = (List&& other) {
            if (this == &other)
                return *this;
            
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
            }
            else {
                throw std::out_of_range("Remove Empty List");
            }
        }

        void InsertBack(const T& data) {
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
        }
        void RemoveBack() {
            if(first_ == nullptr) {
                throw std::out_of_range("Remove Empty List");
            }
            else if(first_ == last_) {
                delete first_;
                first_ = last_ = nullptr;
                return;
            }
            else {
                auto temp_1 = first_;
                auto temp_2 = first_->next;

                while(temp_2 && temp_1) {
                    if(temp_2->next == nullptr)
                        break;
                    temp_1 = temp_2;
                    temp_2 = temp_2->next;
                }

                last_ = temp_1;
                last_->next = nullptr;
                if(temp_2) {
                    delete temp_2;
                }
            }
        }

        inline bool IsEmpty() { return !first_; }
        inline T Front() { if(!first_) throw std::out_of_range("Get Data From Empty List"); return first_->data; }
        inline T Last() { if(!last_) throw std::out_of_range("Get Data From Empty List"); return last_->data; }

    private:
        Node* first_ = nullptr;
        Node* last_ = nullptr;
    };
}
