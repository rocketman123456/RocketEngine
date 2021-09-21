#pragma once
#include <cstdint>
//#include <iostream>
#include <exception>
#include <stdexcept>

// LIFO
namespace Rocket {
    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename T>
    class FixStack {
        public:
        explicit FixStack(int32_t size) : size_(size), data_(new T[size]) {}
        FixStack(const FixStack& stack) {
            this->size_ = stack.size_;
            this->data_ = new T[stack.size_];
            for(int32_t i = 0; i < size_; ++i) {
                this->data_[i] = stack.data_[i];
            }
        }
        FixStack(FixStack&& stack) {
            this->size_ = stack.size_;
            this->data_ = stack.data_;
            stack.size_ = 0;
            stack.data_ = nullptr;
        }
        virtual ~FixStack() {
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
            if (this == &other)
                return *this;
            if(data_)
                delete [] data_;
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

        inline bool IsFull() { return current_ == size_; }
        inline bool IsEmpty() { return current_ == 0; }
        inline int32_t TotalSize() { return size_; }
        inline int32_t CurrentSize() { return current_; }
        inline T* GetData() { return data_; }

        void Resize(int32_t size) {
            if(size < current_)
                throw std::runtime_error("error resize");
            T* temp = new T[size];
            int32_t len = current_;
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
