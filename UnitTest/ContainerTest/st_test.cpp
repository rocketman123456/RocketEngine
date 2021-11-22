#include "Containers/SymbolTable/SequentialSearchST.h"

using namespace Rocket;

int main() {
    SequentialSearchST<int, double> st;

    for(int i = 0; i < 100; ++i) {
        st.put(i, i+1);
    }

    return 0;
}