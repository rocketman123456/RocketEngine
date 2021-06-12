#include "Module/ResourceManager.h"
#include "Parser/ConfigParser.h"

namespace Rocket {
    extern ConfigParser* g_ConfigParser;

    int32_t ResourceManager::Initialize() {
        root_path_ = g_ConfigParser->GetRootPath();
        asset_path_ = root_path_ + "/Asset/";
        std::string type = g_ConfigParser->GetConfigInfo<std::string>("basic", "resource-type");
        RK_CORE_INFO("Resource Type : {}", type);
        return 0;
    }

    void ResourceManager::Finalize() {
        
    }

    void ResourceManager::Tick(TimeStep ts) {
        
    }
}