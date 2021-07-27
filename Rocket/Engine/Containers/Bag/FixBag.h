#pragma once
#include <cstdint>
#include <exception>

namespace Rocket {
    template<typename T> class FixBag;

    template<typename T>
    class FixBagIterator : implements Iterator<T> {
    public:
        explicit FixBagIterator(FixBag<T>* bag) : bag_(bag), data_(bag->data_), size_(bag->current_) {}
        virtual ~FixBagIterator() = default;
        virtual bool HasNext() final { return count_ < size_; }
        virtual T Next() final { T temp = data_[count_]; count_++; return temp; }
    private:
        FixBag<T>* bag_ = nullptr;
        T*      data_ = nullptr;
        int32_t size_ = 0;
        int32_t count_ = 0;
    };

    // TODO : make it thread safe
    template<typename T>
    class FixBag : implements Iterable<T, FixBagIterator<T>> {
        friend class FixBagIterator<T>;
    public:
        explicit FixBag(int32_t size) : size_(size), data_(new T[size]) {}
        FixBag(const FixBag& stack) {
            this->size_ = stack.size_;
            this->data_ = new T[stack.size_];
            for(int32_t i = 0; i < size_; ++i) {
                this->data_[i] = stack.data_[i];
            }
        }
        FixBag(FixBag&& stack) {
            this->size_ = stack.size_;
            this->data_ = stack.data_;
            stack.size_ = 0;
            stack.data_ = nullptr;
        }
        virtual ~FixBag() {
            if(data_) {
                delete [] data_;
            }
            size_ = 0;
        }

        virtual FixBagIterator<T> GetIterator() final {
            return FixBagIterator<T>(this);
        }

        FixBag* operator & () { return this; }
        const FixBag* operator & () const { return this; }

        // Copy
        FixBag& operator = (const FixBag& other) {
            if (this == &other)
                return *this;
            
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
            if(current_ == size_) {
                throw std::out_of_range("Add to Full FixBag");
            }
            data_[current_] = item;
            current_++;
        }

        inline bool IsFull() const { return current_ == size_; }
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
