#include "Console/ConsoleCommandFactory.h"

#include "Console/Command/PrintCommand.h"
#include "Console/Command/ExitCommand.h"

namespace Rocket {
    ConsoleCommandPtr ConsoleCommandFactory::CreateCommand(ConsoleCommandType type, const std::vector<std::string>& args) {
        switch(type) {
            case ConsoleCommandType::PRINT:
                return std::make_shared<PrintCommand>(args);
            case ConsoleCommandType::EXIT:
                return std::make_shared<ExitCommand>(args);
            default:
                return nullptr;
        }
        
    }
}
