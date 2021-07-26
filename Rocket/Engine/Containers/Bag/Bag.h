#pragma once
#include "Pattern/Iterator.h"

#include <cstdint>
#include <exception>

namespace Rocket {
    template<typename T> class Bag;

    template<typename T>
    class BagIterator : implements Iterator<T> {
    public:
        explicit BagIterator(Bag<T>* bag) : bag_(bag), data_(bag->data_), size_(bag->current_) {}
        virtual ~BagIterator() = default;
        virtual bool HasNext() final { return count_ < size_; }
        virtual T Next() final { T temp = data_[count_]; count_++; return temp; }
    private:
        Bag<T>* bag_ = nullptr;
        T*      data_ = nullptr;
        int32_t size_ = 0;
        int32_t count_ = 0;
    };

    // TODO : make it thread safe
    // TODO : make stack implements iterator
    template<typename T>
    class Bag : implements Iterable<T, BagIterator<T>> {
        friend class BagIterator<T>;
    public:
        explicit Bag() : data_(new T[2]), size_(2) {}
        explicit Bag(int32_t size) : data_(new T[size]), size_(size) {}
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

        virtual BagIterator<T> GetIterator() final {
            return BagIterator<T>(this);
        }

        Bag* operator & () { return this; }
        const Bag* operator & () const { return this; }

        // Copy
        Bag& operator = (const Bag& other) {
            if (this == &other)
                return *this;
            if(data_)
                delete [] data_;
            this->data_ = new T[other.size_];
            this->size_ = other.size_;
            for(int32_t i = 0; i < size_; ++i) {
                this->data_[i] = other.data_[i];
            }
            return *this;
        }
        // Move
        Bag& operator = (Bag&& other) {
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

        void Add(const T& item) {
            // Auto Resize
            if(current_ == size_) {
                Resize(size_ * 2);
            }
            data_[current_] = item;
            current_++;
        }

        inline bool IsEmpty() const { return current_ == 0; }
        inline int32_t TotalSize() const { return size_; }
        inline int32_t CurrentSize() const { return current_; }
        inline T* GetData() const { return data_; }

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
