#include "Utils/Hashing.h"

#include <iostream>

using namespace Rocket;
using namespace std;

int main() {
    uint64_t h1 = "test hash"_hash;
    uint64_t h2 = Rocket::hash(std::string("test hash"));
    uint64_t h3 = "test hash"_u64;
    uint64_t h4 = Rocket::u64(std::string("test hash"));

    cout << "test hash: static: " << h1 << endl;
    cout << "test hash: runtime: " << h2 << endl;
    cout << "test hash: static: " << h3 << endl;
    cout << "test hash: runtime: " << h4 << endl;

    return 0;
}