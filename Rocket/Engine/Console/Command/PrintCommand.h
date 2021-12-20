#pragma once
#include "Console/ConsoleCommand.h"

namespace Rocket {
    class PrintCommand : _implements_ ConsoleCommand {
    public:
        PrintCommand(const std::vector<std::string>& args) : ConsoleCommand(args) {}
        virtual ~PrintCommand() = default;
        void Execute() final;
    };
    CLASS_PTR(PrintCommand);
}
