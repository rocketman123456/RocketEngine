#pragma once
#include "Core/Declare.h"

namespace Rocket {
    Interface Execption {
    public:
        virtual ~Execption() = default;
        virtual void what() const = 0;  //获取具体的错误信息
    };

    // TODO : replace std::execption s
    class OutOfRange : implements Execption {
    public:
        OutOfRange(): flag_(1) {}
        OutOfRange(int len, int index): flag_(2), len_(len), index_(index) {}
        OutOfRange(const char* err): flag_(3), err_(err) {}
        virtual ~OutOfRange() = default;
    public:
        void what() const final;  //获取具体的错误信息
    private:
        int flag_;      //不同的flag表示不同的错误
        int len_;       //当前数组的长度
        int index_;     //当前使用的数组下标
        const char* err_;
    };

    class NewSizeError : implements Execption {
    public:
        NewSizeError(int flag, int size) : flag_(flag), size_(size) {} 
        void what() const final;
    private:
        int flag_;
        int size_;
    };
}
