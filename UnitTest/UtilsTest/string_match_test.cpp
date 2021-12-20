#include "Utils/StringUtils.h"

#include <iostream>

using namespace Rocket;

int main() {
    std::string left = "LightTexture";
    std::string right = "BrightTexture";

    auto result = StringMatch(left, right);

    std::cout << "Match Result : " << result << std::endl;

    return 0;
}