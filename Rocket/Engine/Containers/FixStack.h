#pragma once
#include <cstdint>
//#include <iostream>
#include <exception>

// LIFO
namespace Rocket {
    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename T>
    class FixStack {
        public:
        explicit FixStack(int32_t size) {
            this->data_ = new T[size];
            this->size_ = size;
        }
        FixStack(const FixStack& stack) {
            this->data_ = new T[stack.size_];
            this->size_ = stack.size_;
            for(int32_t i = 0; i < size_; ++i) {
                this->data_[i] = stack.data_[i];
            }
        }
        FixStack(FixStack&& stack) {
            this->data_ = stack.data_;
            this->size_ = stack.size_;
            stack.data_ = nullptr;
            stack.size_ = 0;
        }
        virtual ~Stack() {
            if(data_) {
                delete [] data_;
            }
            size_ = 0;
        }

        FixStack* operator & () { return this; }
        const FixStack* operator & () const { return this; }

        // Copy
        FixStack& operator = (const FixStack& other) {
            if(data_) {
                delete [] data_;
            }
            this->data_ = new T[other.size_];
            this->size_ = other.size_;
            for(int32_t i = 0; i < size_; ++i) {
                this->data_[i] = other.data_[i];
            }
            return *this;
        }
        // Move
        FixStack& operator = (FixStack&& other) {
            if(data_) {
                delete [] data_;
            }
            this->data_ = other.data_;
            this->size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
            return *this;
        }

        void Push(const T& item) {
            if(current_ == size_) {
                throw std::out_of_range("Push Full FixStack");
            }
            data_[current_] = item;
            current_++;
        }
        T Pop() {
            if(current_ == 0) {
                throw std::out_of_range("Pop Empty FixStack");
            }
            else {
                current_--;
                return data_[current_];
            }
        }

        bool IsFull() { return current_ == size_; }
        bool IsEmpty() { return current_ == 0; }
        int32_t TotalSize() { return size_; }
        int32_t CurrentSize() { return current_; }
        T* GetData() { return data_; }

        void Resize(int32_t size) {
            //std::cout << "Stack Resize To : " << size << std::endl;
            T* temp = new T[size];
            int32_t len = std::min(size, size_);
            for(int32_t i = 0; i < len; ++i) {
                temp[i] = data_[i];
            }
            delete [] data_;
            data_ = temp;
            size_ = size;
        }

    private:
        int32_t current_ = 0;
        int32_t size_ = 0;
        T*      data_ = nullptr;
    };
}
