#pragma once
#include <cstdint>
//#include <iostream>

namespace Rocket {
    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename T>
    class Bag {
    public:
        explicit Bag() : data_(new T[2]), size_(2) {}
        Bag(const Bag& bag) {
            this->data_ = new T[bag.size_];
            this->size_ = bag.size_;
            for(int32_t i = 0; i < size_; ++i) {
                this->data_[i] = bag.data_[i];
            }
        }
        Bag(Bag&& bag) {
            this->data_ = bag.data_;
            this->size_ = bag.size_;
            bag.data_ = nullptr;
            bag.size_ = 0;
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
            return *this;
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
            return *this;
        }

        void Add(const T& item) {
            // Auto Resize
            if(current_ == size_) {
                Resize(size_ * 2);
            }
            data_[current_] = item;
            current_++;
        }

        inline bool IsEmpty() { return current_ == 0; }
        inline int32_t TotalSize() { return size_; }
        inline int32_t CurrentSize() { return current_; }
        inline T* GetData() { return data_; }

        void Resize(int32_t size) {
            //std::cout << "Bag Resize To : " << size << std::endl;
            T* temp = new T[size];
            int32_t length = std::min(size, current_);
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
