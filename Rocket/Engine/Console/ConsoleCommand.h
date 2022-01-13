#pragma once
#include "Command/Command.h"

#include <vector>
#include <string>

namespace Rocket {
    enum class ConsoleCommandType : int32_t {
        PRINT = 0,
        EXIT,
        ADD,
        SUB,
    };

    enum class ConsoleVariableType : int32_t {
        NONE = 0,
        BOOL,
        INT8,   UINT8,
        INT16,  UINT16,
        INT32,  UINT32,
        INT64,  UINT64,
        FLOAT,  DOUBLE,
        STRING,
    };

    _Interface_ ConsoleCommand : _inherits_ Command {
    public:
        ConsoleCommand(const std::vector<std::string>& args) : arguments(args) {}
        virtual ~ConsoleCommand() = default;
        std::vector<std::string> arguments;
        std::vector<ConsoleVariableType> types;
    };

    CLASS_PTR(ConsoleCommand);
}
