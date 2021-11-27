#include "Memory/MemoryCheck.h"
#include "Config/ConfigSetting.h"
#include "Parser/ImageParser.h"
#include "Utils/FindRootDir.h"
#include "Log/Log.h"

#include <iostream>

using namespace Rocket;

int main() {
    //Rocket::Log::Init();

    std::string name = "_root_dir_";
    auto root = FindRootDir(name);
    root += "/";

    {
        ImageParser parser(root, "Asset/Model/rock/rock.png");

        std::vector<Eigen::Vector4f> data;
        parser.Parse(data);
        
        std::cout << "Size: " << data.size() << std::endl;
    }

    //Rocket::Log::End();
    return 0;
}