#include "Config/ConfigSetting.h"
#include "Utils/FindRootDir.h"
#include "Log/Log.h"

#include "Parser/ObjParser.h"

#include <iostream>

using namespace Rocket;

int main() {
    //Rocket::Log::Init();

    std::string name = "_root_dir_";
    auto root = FindRootDir(name);
    root += "/";

    {
        std::string name = "Asset/Model/cube/cube.obj";
        ObjParser parser(root, name);
        parser.Initialize();
        auto content = parser.GetContent();
        std::cout << "File Content:\n" << content << std::endl << std::endl;
        parser.Parse();
        std::cout << "Meshes Size: " << parser.loaded_meshes.size() << std::endl << std::endl;
        std::cout << "Vertices Size: " << parser.loaded_vertices.size() << std::endl << std::endl;
        std::cout << "Indices Size: " << parser.loaded_indices.size() << std::endl << std::endl;
        std::cout << "Materials Size: " << parser.loaded_materials.size() << std::endl << std::endl;
    }

    //Rocket::Log::End();
    return 0;
}