#include "Parser/ConfigParser.h"

namespace Rocket {
    ConfigParser::ConfigParser() {
        FindRootDir();
    }
    
    void ConfigParser::FindRootDir() {
        root_dir_ = "";
    }
}
