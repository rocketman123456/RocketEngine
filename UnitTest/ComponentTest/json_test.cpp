#include "Parser/JsonParser.h"
#include "Utils/FindRootDir.h"
#include "Log/Log.h"

#include <iostream>

using namespace Rocket;

int main() {
    //Log::Init();

    std::string root = FindRootDir("_root_dir_");

    JsonParser parser;

    parser.Initialize(root, "/Asset/Note/Castle in the Sky.json");

    //Log::End();
    return 0;
}