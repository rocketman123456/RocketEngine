#include "Containers/SymbolTable/SequentialSearchST.h"
#include "Containers/SymbolTable/BinarySearchST.h"
#include "Containers/SymbolTable/SeparateChainingHashST.h"
#include "Containers/SymbolTable/SymbolTable.h"
// #include "Containers/SymbolTable/LinearProbingHashST.h"

using namespace Rocket;

#include <iostream>

int main() {
    SequentialSearchST<int, double> ss_st;
    BinarySearchST<int, double> bs_st;
    SeparateChainingHashST<int, double> sch_st;
    SymbolTable<int, double> st_st;
    // LinearProbingHashST<int, double> lph_st;

    for(int i = 0; i < 100; ++i) {
        ss_st.put(i, i+1);
        bs_st.put(i, i+1);
        sch_st.put(i, i+1);
        st_st.put(i, i+1);
        // lph_st.put(i, i+1);
    }

    std::cout << sch_st.get(2) << std::endl;
    std::cout << bs_st.ceiling(2) << std::endl;
    std::cout << bs_st.max() << std::endl;

    return 0;
}