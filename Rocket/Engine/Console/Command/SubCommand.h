#pragma once
#include "Console/ConsoleCommand.h"

namespace Rocket {
    class SubCommand : _implements_ ConsoleCommand {
    public:
        SubCommand(const std::vector<std::string>& args) : ConsoleCommand(args) {
            types = {
                ConsoleVariableType::DOUBLE,
                ConsoleVariableType::DOUBLE,
            };
        }
        virtual ~SubCommand() = default;
        void Execute() final;
    };
    CLASS_PTR(SubCommand);
}
