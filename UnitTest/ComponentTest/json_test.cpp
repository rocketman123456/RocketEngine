#include "Parser/JsonParser.h"
#include "Log/Log.h"
#include "Utils/FindRootDir.h"

using namespace Rocket;

int main() {
    Log::Init();

    std::string root = FindRootDir("_root_dir_");

    JsonParser parser;

    parser.Initialize("true");
    parser.Finalize();

    Log::End();
    return 0;
}