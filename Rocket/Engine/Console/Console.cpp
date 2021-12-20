#include "Console/Console.h"
#include "Console/ConsoleCommandFactory.h"
#include "Utils/StringUtils.h"
#include "Log/Log.h"

namespace Rocket {
    void Console::Register(const std::string& name, ConsoleCommandType type) {
        command_list[name] = type;
    }

    void Console::Unregister(const std::string& name) {
        auto found = command_list.find(name);
        command_list.erase(found);
    }

    void Console::Input(const std::string& input) {
        SplitSingleChar(input, &arguments, ' ');
        if(arguments.size() == 0) {
            RK_ERROR(Console, "Invald Arguments");
            return;
        }
        auto found = command_list.find(input);
        if(found == command_list.end()) {
            RK_ERROR(Console, "Invald Command");
            return;
        }
    }

    void Console::ParseCommand() {
    }

    void Console::Execute() {
        current_command->Execute();
    }
}
