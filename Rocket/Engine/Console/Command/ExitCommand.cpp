#include "Console/Command/ExitCommand.h"

#include <iostream>

namespace Rocket {
    void ExitCommand::Execute() {
        exit(0);
    }
}