//https://vorbrodt.blog/2019/01/28/enum-to-string/
//https://vorbrodt.blog/2019/01/29/enum-to-string-take-2/

#include <iostream>
#include <map>
#include <cassert>

enum class MyEnum : int
{
    V1,
    V2,
    V3,
    SIZE
};

const char* MyEnumToString(MyEnum e)
{
    using MapType = std::map<MyEnum, const char*>;
    static const MapType kMap =
    {
        { MyEnum::V1, "V1" },
        { MyEnum::V2, "V2" },
        { MyEnum::V3, "V3" },
    };
    assert(kMap.size() == (int)MyEnum::SIZE);
    return kMap.at(e);
}

#define MY_ENUM_2 \
    X(V1) \
    X(V2) \
    X(V3)

#define X(name) name,
enum MyEnum_2
{
    MY_ENUM_2
};
#undef X

constexpr const char* MyEnumToString_2(MyEnum_2 e) noexcept
{
    #define X(name) case(name): return #name;
    switch(e)
    {
        MY_ENUM_2
    }
    #undef X
}

int main(int argc, char** argv)
{
    std::cout << MyEnumToString(MyEnum::V1) << std::endl;
    std::cout << MyEnumToString(MyEnum::V2) << std::endl;
    std::cout << MyEnumToString(MyEnum::V3) << std::endl;

    std::cout << MyEnumToString_2(V1) << std::endl;
    std::cout << MyEnumToString_2(V2) << std::endl;
    std::cout << MyEnumToString_2(V3) << std::endl;
    return 0;
}
