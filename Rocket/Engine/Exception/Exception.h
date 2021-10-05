#pragma once
#include "Core/Declare.h"

namespace Rocket {
    _Interface_ Execption {
    public:
        virtual ~Execption() = default;
        virtual void what() const = 0;
    };

    // TODO : replace std::execption s
    class OutOfRange : _implements_ Execption {
    public:
        OutOfRange(): flag_(1) {}
        OutOfRange(int len, int index): flag_(2), len_(len), index_(index) {}
        OutOfRange(const char* err): flag_(3), err_(err) {}
        virtual ~OutOfRange() = default;
    public:
        void what() const final;
    private:
        int flag_;
        int len_;
        int index_;
        const char* err_;
    };

    class NewSizeError : _implements_ Execption {
    public:
        NewSizeError(int flag, int size) : flag_(flag), size_(size) {} 
        void what() const final;
    private:
        int flag_;
        int size_;
    };
}
