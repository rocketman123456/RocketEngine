#include "Memory/MemoryCheck.h"
#include "Containers/Bag/Bag.h"
#include "Containers/Bag/FixBag.h"
#include "Containers/Bag/ListBag.h"
#include <random>

using namespace Rocket;

int main() {
    {
        int count = 10000;
        Bag<int32_t> bag;
        for(int i = 0; i < count; ++i) {
            bag.Add(std::rand());
        }
        auto iterator = bag.GetIterator();
        int iter_count = 0;
        while(iterator.HasNext()) {
            [[maybe_unused]] int32_t temp = iterator.Next();
            //std::cout << "Iterator Get: " << temp << std::endl;
            iter_count++;
        }
        std::cout << "Bag Current Size: " << bag.CurrentSize() << std::endl;
        std::cout << "Bag Iterator Count: " << iter_count << std::endl;
    }
    {
        FixBag<double> bag_f(1000);
        for(int i = 0; i < 1000; ++i) {
            bag_f.Add(std::rand());
        }
        auto iterator_f = bag_f.GetIterator();
        int iter_count = 0;
        while(iterator_f.HasNext()) {
            [[maybe_unused]] int32_t temp = iterator_f.Next();
            iter_count++;
        }
        std::cout << "FixBag Current Size: " << bag_f.CurrentSize() << std::endl;
        std::cout << "FixBag Iterator Count: " << iter_count << std::endl;
    }
    {
        ListBag<double> bag_l;
        for(int i = 0; i < 1000; ++i) {
            bag_l.Add(std::rand());
        }
        auto iterator_f = bag_l.GetIterator();
        int iter_count = 0;
        while(iterator_f.HasNext()) {
            [[maybe_unused]] int32_t temp = iterator_f.Next();
            iter_count++;
        }
        std::cout << "ListBag Current Size: " << bag_l.CurrentSize() << std::endl;
    }
    return 0;
}