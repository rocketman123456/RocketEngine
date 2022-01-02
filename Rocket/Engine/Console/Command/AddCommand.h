#pragma once
#include "Console/ConsoleCommand.h"

namespace Rocket {
    class AddCommand : _implements_ ConsoleCommand {
    public:
        AddCommand(const std::vector<std::string>& args) : ConsoleCommand(args) {
            types = {
                ConsoleVariableType::DOUBLE,
                ConsoleVariableType::DOUBLE,
            };
        }
        virtual ~AddCommand() = default;
        void Execute() final;
    };
    CLASS_PTR(AddCommand);
}
