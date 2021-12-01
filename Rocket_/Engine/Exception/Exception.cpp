#include "Exception/Exception.h"
#include "Log/Log.h"

#include <iostream>

namespace Rocket {
    void OutOfRange::what() const {
        if(flag_ == 1) {
            std::cout << "Error: empty array, no elements to pop." << std::endl;
        } else if(flag_ == 2) {
            std::cout << "Error: out of range( array length " << len_ << ", access index " << index_ << " )" << std::endl;
        } else if(flag_ == 3) {
            std::cout << "Error: " << err_ << std::endl;
        } else {
            std::cout << "Unknown exception." << std::endl;
        }
    }

    void NewSizeError::what() const {
        if(flag_ == 1) {
            std::cout << "New Allocation Size " << size_ << " Too Small." << std::endl;
        } else if(flag_ == 2) {
            std::cout << "New Allocation Size " << size_ << " Failed." << std::endl;
        } else {
            std::cout << "Unknown exception." << std::endl;
        }
    }
}
