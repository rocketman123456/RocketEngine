#pragma once
#include "Console/ConsoleCommand.h"

namespace Rocket {
    class ExitCommand : _implements_ ConsoleCommand {
    public:
        ExitCommand(const std::vector<std::string>& args) : ConsoleCommand(args) {}
        virtual ~ExitCommand() = default;
        void Execute() final;
    };
    CLASS_PTR(ExitCommand);
}
