#include "Memory/MemoryCheck.h"
#include "Containers/Bag/Bag.h"
#include "Containers/Bag/FixedBag.h"
#include "Containers/Bag/ConcurrentBag.h"

#include <random>
#include <iostream>

using namespace Rocket;

int main() {
    {
        int count = 10000;
        Bag<double> bag;
        for(int i = 0; i < count * 2; ++i) {
            bag.add(std::rand());
        }
        auto iterator = bag.begin();
        for(auto iterator = bag.begin(); iterator != bag.end(); ++iterator) {
            double temp = *iterator;
            //std::cout << "Iterator Get: " << temp << std::endl;
        }
        std::cout << "Bag Current Size: " << bag.size() << std::endl;
    }
    {
        const int count = 10000;
        FixedBag<double, count> bag;
        for(int i = 0; i < count * 2; ++i) {
            bag.add(std::rand());
        }
        auto iterator = bag.begin();
        for(auto iterator = bag.begin(); iterator != bag.end(); ++iterator) {
            double temp = *iterator;
            //std::cout << "Iterator Get: " << temp << std::endl;
        }
        std::cout << "Bag Current Size: " << bag.size() << std::endl;
    }
    {
        const int count = 10000;
        ConcurrentBag<double> bag;
        for(int i = 0; i < count * 2; ++i) {
            bag.add(std::rand());
        }
        auto iterator = bag.begin();
        for(auto iterator = bag.begin(); iterator != bag.end(); ++iterator) {
            double temp = *iterator;
            //std::cout << "Iterator Get: " << temp << std::endl;
        }
        std::cout << "Bag Current Size: " << bag.size() << std::endl;
    }
    return 0;
}
