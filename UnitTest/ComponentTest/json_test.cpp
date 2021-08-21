#include "Parser/JsonParser.h"
#include "Log/Log.h"
#include "Utils/FindRootDir.h"

#include <iostream>

using namespace Rocket;

int main() {
    Log::Init();

    std::string root = FindRootDir("_root_dir_");

    JsonParser parser;

    parser.Initialize(root, "/Asset/Note/Castle in the Sky.json");

    std::cout << " \"Hello\" " << std::endl;
    int32_t result = parser.Initialize(" \"Hello\" ");
    parser.Finalize();

    auto value = parser.GetJsonValue()->value_s;
    std::cout << value << "," << (value == "Hello") << std::endl;

    Log::End();
    return 0;
}