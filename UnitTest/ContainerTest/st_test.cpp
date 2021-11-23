#include "Containers/SymbolTable/SequentialSearchST.h"
#include "Containers/SymbolTable/BinarySearchST.h"

using namespace Rocket;

int main() {
    SequentialSearchST<int, double> ss_st;
    BinarySearchST<int, double> bs_st;

    for(int i = 0; i < 100; ++i) {
        ss_st.put(i, i+1);
        bs_st.put(i, i+1);
    }

    return 0;
}