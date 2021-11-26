#include "Containers/SymbolTable/SequentialSearchST.h"
#include "Containers/SymbolTable/BinarySearchST.h"

using namespace Rocket;

#include <iostream>

int main() {
    SequentialSearchST<int, double> ss_st;
    BinarySearchST<int, double> bs_st;

    for(int i = 0; i < 100; ++i) {
        ss_st.put(i, i+1);
        bs_st.put(i, i+1);
    }

    std::cout << bs_st.get(2) << std::endl;
    std::cout << bs_st.ceiling(2) << std::endl;
    std::cout << bs_st.max() << std::endl;

    return 0;
}