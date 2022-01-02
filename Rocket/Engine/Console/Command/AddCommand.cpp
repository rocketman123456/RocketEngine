#include "Console/Command/AddCommand.h"

#include <iostream>
#include <exception>

namespace Rocket {
    void AddCommand::Execute() {
        if(arguments.size() != types.size()) {
            std::cout << "arguments SIZE error, should be 2";
        }
        try {
            double a1 = std::stod(arguments.at(0));
            double a2 = std::stod(arguments.at(1));
            double add = a1 + a2;
            std::cout << arguments.at(0) << " + " << arguments.at(1) << " = " << add << std::endl;
        } catch(std::exception e) {
            std::cout << e.what() << std::endl;
        }
    }
}
