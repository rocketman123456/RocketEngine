#include "Console/Console.h"

using namespace Rocket;

#include <iostream>

int main() {
    Console console;
    console.SetCapacity(100);

    console.Register("print", ConsoleCommandType::PRINT);
    console.Register("exit", ConsoleCommandType::EXIT);
    console.Register("add", ConsoleCommandType::ADD);
    console.Register("sub", ConsoleCommandType::SUB);

    console.RegisterUsage("print", "Print Arguments");
    console.RegisterUsage("exit", "Exit Program");
    console.RegisterUsage("add", "Print a + b");
    console.RegisterUsage("sub", "Print a - b");

    console.PrintUsage();

    while(true) {
        std::cout << "Input Command: ";
        const int MAX_SIZE = 1000;
        char cmd[MAX_SIZE];
        std::cin.getline(cmd, MAX_SIZE);
        console.Input(cmd);
    }

    return 0;
}
