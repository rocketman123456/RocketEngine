#include "Console/ConsoleCommandFactory.h"

#include "Console/Command/PrintCommand.h"
#include "Console/Command/ExitCommand.h"
#include "Console/Command/AddCommand.h"
#include "Console/Command/SubCommand.h"

namespace Rocket {
    ConsoleCommandPtr ConsoleCommandFactory::CreateCommand(ConsoleCommandType type, 
            const std::vector<std::string>& args) {
        switch(type) {
            case ConsoleCommandType::PRINT:
                return std::make_shared<PrintCommand>(args);
            case ConsoleCommandType::EXIT:
                return std::make_shared<ExitCommand>(args);
            case ConsoleCommandType::ADD:
                return std::make_shared<AddCommand>(args);
            case ConsoleCommandType::SUB:
                return std::make_shared<SubCommand>(args);
            default:
                return nullptr;
        }
    }

    // static ConsoleCommandPtr CreateCommand(ConsoleCommandType type, 
    //         const std::vector<std::string>& args, 
    //         const std::vector<ConsoleVariableType>& types) {
    //     switch(type) {
    //         case ConsoleCommandType::PRINT:
    //             return std::make_shared<PrintCommand>(args);
    //         case ConsoleCommandType::EXIT:
    //             return std::make_shared<ExitCommand>(args);
    //         default:
    //             return nullptr;
    //     }
    // }
}
