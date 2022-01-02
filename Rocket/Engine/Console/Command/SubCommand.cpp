#include "Console/Command/SubCommand.h"

#include <iostream>
#include <exception>

namespace Rocket {
    void SubCommand::Execute() {
        if(arguments.size() != types.size()) {
            std::cout << "arguments SIZE error, should be 2";
        }
        try {
            double a1 = std::stod(arguments.at(0));
            double a2 = std::stod(arguments.at(1));
            double sub = a1 - a2;
            std::cout << arguments.at(0) << " - " << arguments.at(1) << " = " << sub << std::endl;
        } catch(std::exception e) {
            std::cout << e.what() << std::endl;
        }
    }
}
