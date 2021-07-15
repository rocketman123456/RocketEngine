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
        }

        // Copy
        List& operator = (const List& other) {
            // delete exist data first
            while(first_) {
                auto temp = first_;
                first_ = first_->next;
                delete temp;
            }
            first_ = last_ = nullptr;
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
            }
        }
        // Move
        List& operator = (List&& other) {
            first_ = other.first_;
            last_ = other.last_;
            other.first_ = nullptr;
            other.first_last_ = nullptr;
        }

        void InsertFront(const T& data) {
            auto temp = new Node;
            temp->data = data;
            temp->next = first_;
            first_ = temp;
            if(!last_) {
                last_ = temp;
            }
        }
        void InsertBack(const T& data) {
            auto temp = new Node;
            if(last_) {
                last_->next = temp;
            }
            if(!first_) {
                first_ = temp;
            }
            temp->data = data;
            temp->next = nullptr;
            last_ = temp;
            
        }
        void RemoveFront() {
            auto temp = first_;
        }
        void RemoveBack() {
            auto temp_1 = first_;
            auto temp_2 = first_->next;
        }


    private:
        Node* first_ = nullptr;
        Node* last_ = nullptr;
    };
}
