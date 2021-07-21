#pragma once
#include <cstdint>
//#include <iostream>
#include <exception>

namespace Rocket {
    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename T>
    class FixBag {
    public:
        explicit FixBag(int32_t size) : data_(new T[size]), size_(size) {}
        FixBag(const FixBag& stack) {
            this->data_ = new T[stack.size_];
            this->size_ = stack.size_;
            for(int32_t i = 0; i < size_; ++i) {
                this->data_[i] = stack.data_[i];
            }
        }
        FixBag(FixBag&& stack) {
            this->data_ = stack.data_;
            this->size_ = stack.size_;
            stack.data_ = nullptr;
            stack.size_ = 0;
        }
        virtual ~FixBag() {
            if(data_) {
                delete [] data_;
            }
            size_ = 0;
        }

        FixBag* operator & () { return this; }
        const FixBag* operator & () const { return this; }

        // Copy
        FixBag& operator = (const FixBag& other) {
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
        FixBag& operator = (FixBag&& other) {
            if(data_) {
                delete [] data_;
            }
            this->data_ = other.data_;
            this->size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
            return *this;
        }

        void Add(const T& item) {
            if(current_ == size_) {
                throw std::out_of_range("Add to Full FixBag");
            }
            data_[current_] = item;
            current_++;
        }

        inline bool IsFull() { return current_ == size_; }
        inline bool IsEmpty() { return current_ == 0; }
        inline int32_t TotalSize() { return size_; }
        inline int32_t CurrentSize() { return current_; }
        inline T* GetData() { return data_; }

        void Resize(int32_t size) {
            if(size < current_)
                throw std::bad_array_new_length();
            T* temp = new T[size];
            int32_t length = current_;
            for(int32_t i = 0; i < length; ++i) {
                temp[i] = data_[i];
            }
            auto temp_data = data_;
            delete [] temp_data;
            data_ = temp;
            size_ = size;
        }

    private:
        int32_t current_ = 0;
        int32_t size_ = 0;
        T*      data_ = nullptr;
    };
}
