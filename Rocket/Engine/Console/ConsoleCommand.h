#pragma once
#include "Command/Command.h"

#include <vector>
#include <string>

namespace Rocket {
    enum class ConsoleCommandType : int32_t {
        PRINT = 0,
    };

    _Interface_ ConsoleCommand : _inherits_ Command {
    public:
        ConsoleCommand(const std::vector<std::string>& args) : arguments(args) {}
        virtual ~ConsoleCommand() = default;
        std::vector<std::string> arguments;
    };

    CLASS_PTR(ConsoleCommand);
}