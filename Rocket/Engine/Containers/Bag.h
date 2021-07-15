#pragma once
#include <cstdint>
#include <iostream>

namespace Rocket {
    template<typename T>
    class Bag {
    public:
        explicit Bag() {
            this->data_ = new T[2];
            this->size_ = 2;
        }
        explicit Bag(int32_t size) {
            this->data_ = new T[size];
            this->size_ = size;
        }
        explicit Bag(const Bag& stack) {
            this->data_ = new T[stack.size_];
            this->size_ = stack.size_;
            for(int32_t i = 0; i < size_; ++i) {
                this->data_[i] = stack.data_[i];
            }
        }
        explicit Bag(Bag&& stack) {
            this->data_ = stack.data_;
            this->size_ = stack.size_;
            stack.data_ = nullptr;
            stack.size_ = 0;
        }
        virtual ~Bag() {
            if(data_) {
                delete [] data_;
            }
            size_ = 0;
        }

        Bag* operator & () { return this; }
        const Bag* operator & () const { return this; }

        // Copy
        Bag& operator = (const Bag& other) {
            if(data_) {
                delete [] data_;
            }
            this->data_ = new T[other.size_];
            this->size_ = other.size_;
            for(int32_t i = 0; i < size_; ++i) {
                this->data_[i] = other.data_[i];
            }
        }
        // Move
        Bag& operator = (Bag&& other) {
            if(data_) {
                delete [] data_;
            }
            this->data_ = other.data_;
            this->size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }

        void Add(const T& item) {
            // Auto Resize
            if(current_ == size_ - 1) {
                Resize(size_ * 2);
            }
            data_[current_] = item;
            current_++;
        }

        bool IsEmpty() { return current_ == 0; }
        int32_t TotalSize() { return size_; }
        int32_t CurrentSize() { return current_; }
        T* GetData() { return data_; }

    private:
        void Resize(int32_t size) {
            std::cout << "Bag Resize To : " << size << std::endl;
            T* temp = new T[size];
            int32_t len = std::min(size, size_);
            for(int32_t i = 0; i < len; ++i) {
                temp[i] = data_[i];
            }
            delete [] data_;
            data_ = temp;
            size_ = size;
        }

        int32_t current_ = 0;
        int32_t size_ = 0;
        T*      data_ = nullptr;
    };
}