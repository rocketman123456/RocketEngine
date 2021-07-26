#pragma once
#include <exception>

// FIFO
namespace Rocket {
    template<typename T>
    class Queue {
        struct Node {
            Node* next;
            T data;
        };
    public:
        explicit Queue() : first_(nullptr), last_(nullptr), size_(0) {}
        Queue(const Queue& queue) {
            // delete exist data first
            first_ = last_ = nullptr;
            // skip empty list
            if(!queue.first_)
                return;
            // deep copy list data
            auto temp_first = queue.first_;
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
        Queue(Queue&& queue) {
            first_ = queue.first_;
            last_ = queue.last_;
            queue.first_ = nullptr;
            queue.first_last_ = nullptr;
        }
        virtual ~Queue() {
            while(first_) {
                auto temp = first_;
                first_ = first_->next;
                delete temp;
            }
            first_ = last_ = nullptr;
        }

        Queue* operator & () { return this; }
        const Queue* operator & () const { return this; }

        // Copy
        Queue& operator = (const Queue& other) {
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
        Queue& operator = (Queue&& other) {
            if (this == &other)
                return *this;
            first_ = other.first_;
            last_ = other.last_;
            other.first_ = nullptr;
            other.first_last_ = nullptr;
            return *this;
        }

        void Enqueue(const T& item) {
            Node* old = last_;
            last_ = new Node;
            last_->data = item;
            last_->next = nullptr;
            if(nullptr == first_) {
                first_ = last_;
            }
            else {
                old->next = last_;
            }
            size_++;
        }
        T Dequeue() {
            if(nullptr == first_) {
                throw std::out_of_range("Dequeue Empty Queue");
            }
            else {
                T item = first_->data;
                auto temp = first_;
                first_ = first_->next;
                delete temp;
                if(first_ == nullptr)
                    last_ = nullptr;
                size_--;
                return item;
            }
        }

        inline bool IsEmpty() { return size_ == 0; }
        inline int32_t Size() { return size_; }

    private:
        Node* first_;
        Node* last_;
        int32_t size_;
    };
}
