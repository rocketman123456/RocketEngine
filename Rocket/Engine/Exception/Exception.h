#pragma once
#include <iostream>

namespace Rocket {
    // TODO : replace std::execption s
    class OutOfRange{
    public:
        OutOfRange(): flag_(1) {};
        OutOfRange(int len, int index): len_(len), index_(index), flag_(2) {}
        OutOfRange(const char* err): err_(err), flag_(3) {};
    public:
        void what() const;  //获取具体的错误信息
    private:
        int flag_;      //不同的flag表示不同的错误
        int len_;       //当前数组的长度
        int index_;     //当前使用的数组下标
        const char* err_;
    };

    void OutOfRange::what() const {
        if(flag_ == 1) {
            std::cout << "Error: empty array, no elements to pop." << std::endl;
        }
        else if(flag_ == 2) {
            std::cout << "Error: out of range( array length " << len_ << ", access index " << index_ << " )" << std::endl;
        }
        else if(flag_ == 3) {
            std::cout << "Error: " << err_ << std::endl;
        }
        else {
            std::cout << "Unknown exception." << std::endl;
        }
    }
}
