#include "Pattern/Factory.h"
#include <string>

using namespace Rocket;

class Person {
public:
    Person(const std::string& name) : name_(name) {}
private:
    std::string name_;
};

int main() {
    Factory<Person> factory;

    auto p1 = factory.Create("name 1", "name 1");
    auto p2 = factory.Create("name 2", "name 2");
    auto p3 = factory.Create("name 3", "name 3");
    auto p4 = factory.Create("name 4", "name 4");
    auto p5 = factory.Create("name 5", "name 5");
    auto p6 = factory.Create("name 6", "name 6");

    return 0;
}