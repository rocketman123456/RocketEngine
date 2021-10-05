#pragma once
#include "Command/Command.h"

#include <list>

namespace Rocket {
    class CommandHistory {
    public:
        virtual ~CommandHistory() = default;
    private:
        std::list<CommandPtr> command_record_;
    };
}