#pragma once
#include <exception>

namespace Rocket {
    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename T>
    class ListStack {
        struct Node {
            Node* next;
            T data;
        };
    public:
        explicit ListStack() : first_(nullptr), size_(0) {}
        ListStack(const ListStack& stack) {
            this->size_ = stack.size_;
            this->first_ = nullptr;
            if(!stack.first_)
                return;
            auto temp_first = stack.first_;
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
        ListStack(ListStack&& stack) {
            this->first_ = stack.first_;
            this->size_ = stack.size_;
            stack.first_ = nullptr;
            stack.size_ = 0;
        }
        virtual ~ListStack() {
            if(first_) {
                while(first_) {
                    auto temp = first_;
                    first_ = first_->next;
                    delete temp;
                }
                first_ = nullptr;
            }
            this->size_ = 0;
            this->first_ = nullptr;
            this->last_ = nullptr;
        }

        ListStack* operator & () { return this; }
        const ListStack* operator & () const { return this; }

        // Copy
        ListStack& operator = (const ListStack& other) {
            // delete date
            if(first_) {
                while(first_) {
                    auto temp = first_;
                    first_ = first_->next;
                    delete temp;
                }
                first_ = nullptr;
            }
            this->size_ = other.size_;
            this->first_ = nullptr;
            // skip empty
            if(!other.first_)
                return *this;
            // deep copy
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
                if(first_ == nullptr)
                    first_ = temp;
                temp_first = temp_first->next;
            }
            return *this;
        }
        // Move
        ListStack& operator = (ListStack&& other) {
            if(first_) {
                while(first_) {
                    auto temp = first_;
                    first_ = first_->next;
                    delete temp;
                }
                first_ = nullptr;
            }
            this->first_ = other.first_;
            this->size_ = other.size_;
            other.first_ = nullptr;
            other.size_ = 0;
            return *this;
        }

        void Push(const T& item) {
            // Auto Resize
            Node* old = first_;
            first_ = new Node;
            first_->data = item;
            first_->next = old;
            size_++;
        }
        T Pop() {
            if(size_ == 0) {
                throw std::out_of_range("Pop Empty ListStack");
            }
            else {
                T data = first_->data;
                auto temp = first_;
                first_ = first_->next;
                delete temp;
                size_--;
                return data;
            }
        }

        inline bool IsEmpty() { return first_ == nullptr; }
        inline int32_t TotalSize() { return size_; }

    private:
        int32_t size_ = 0;
        Node*   first_ = nullptr;
        Node*   last_ = nullptr;
    };
}
