#pragma once
#include "Console/ConsoleCommand.h"

namespace Rocket {
    class ConsoleCommandFactory {
    public:
        static ConsoleCommandPtr CreateCommand(ConsoleCommandType type, 
            const std::vector<std::string>& args);
    };
}
