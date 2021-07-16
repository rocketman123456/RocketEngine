#pragma once

namespace Rocket {
    template<typename T>
    class List {
        struct Node {
            Node* next = nullptr;
            T data;
        };
    public:
        explicit List() {
            first_ = nullptr;
            last_ = nullptr;
        }
        explicit List(const List& list) {
            // delete exist data first
            while(first_) {
                auto temp = first_;
                first_ = first_->next;
                delete temp;
            }
            first_ = last_ = nullptr;
            if(!list.first_)
                return;
            // deep copy list data
            auto temp_first = list.first_;
            auto temp = new Node;
            first_ = last_ = temp;
            temp->data = temp_first->data;
            temp->next = nullptr;
            temp_first = temp_first->next;
            while(temp_first) {
                auto temp = new Node;
                temp->data = temp_first->data;
                temp->next = nullptr;
                last_->next = temp;
                last_ = temp;
                temp_first = temp_first->next;
            }
        }
        explicit List(List&& list) {
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
            // delete exist data first
            while(first_) {
                auto temp = first_;
                first_ = first_->next;
                delete temp;
            }
            first_ = last_ = nullptr;
            if(!other.first_)
                return *this;
            // deep copy list data
            auto temp_first = other.first_;
            auto temp = new Node;
            first_ = last_ = temp;
            temp->data = temp_first->data;
            temp->next = nullptr;
            temp_first = temp_first->next;
            while(temp_first) {
                auto temp = new Node;
                temp->data = temp_first->data;
                temp->next = nullptr;
                last_->next = temp;
                last_ = temp;
                temp_first = temp_first->next;
            }
            return *this;
        }
        // Move
        List& operator = (List&& other) {
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
            auto temp = first_;
            if(first_) {
                first_ = first_->next;
                delete temp;
                if(first_ == nullptr) {
                    first_ = last_ = nullptr;
                }
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
                return;
            }
            else if(first_ == last_ && first_ != nullptr) {
                delete first_;
                first_ = last_ = nullptr;
                return;
            }

            auto temp_1 = first_;
            auto temp_2 = first_->next;

            while(temp_2 && temp_1) {
                auto temp = temp_2->next;
                if(!temp)
                    break;
                temp_1 = temp_2;
                temp_2 = temp_2->next;
            }

            last_ = temp_1;
            temp_1->next = nullptr;
            if(temp_2) {
                delete temp_2;
            }
        }

        bool IsEmpty() { return !first_; }
        // should use when list is not empty
        T Front() { return first_->data; }
        T Last() { return last_->data; }

    private:
        Node* first_ = nullptr;
        Node* last_ = nullptr;
    };
}
