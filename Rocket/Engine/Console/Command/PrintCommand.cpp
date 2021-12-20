#include "Console/Command/PrintCommand.h"

#include <iostream>

namespace Rocket {
    void PrintCommand::Execute() {
        std::cout << "arguments: ";
        for(auto arg : arguments) {
            std::cout << arg << " ";
        }
        std::cout << std::endl;
    }
}