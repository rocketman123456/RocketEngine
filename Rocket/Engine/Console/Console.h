#pragma once
#include "Console/ConsoleCommand.h"

#include <string>
#include <list>
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace Rocket {
    class Console {
    public:
        inline void SetCapacity(std::size_t c) { capacity = c; }
        inline const std::string& GetCurrentInput() { return current_input; }
        inline const std::list<std::string>& GetInputBuffer() { return input_buffer; }
        inline const ConsoleCommandPtr& GetCurrentCommand() { return current_command; }
        inline const std::list<ConsoleCommandPtr>& GetCommandBuffer() { return command_nuffer; }

        void Register(const std::string& name, ConsoleCommandType type);
        void Unregister(const std::string& name);
        void Input(const std::string& input);
    private:
        void ParseCommand();
        void FindBestMatch();
        void Execute();
    private:
        std::size_t capacity = 0;
        std::unordered_map<std::string, ConsoleCommandType> command_list = {};
        std::list<std::string> input_buffer = {};
        std::list<ConsoleCommandPtr> command_nuffer = {};
        std::string current_input = {};
        ConsoleCommandPtr current_command = nullptr;
        std::string command_name = "";
        ConsoleCommandType command_type;
        std::vector<std::string> arguments = {};
    };
}
