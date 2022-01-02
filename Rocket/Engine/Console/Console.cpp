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

    void Console::RegisterUsage(const std::string& name, const std::string& usage) {
        command_usage_list[name] = usage;
    }

    void Console::UnregisterUsage(const std::string& name) {
        auto found = command_usage_list.find(name);
        command_usage_list.erase(found);
    }

    void Console::PrintUsage() {
        RK_INFO(Console, "Console Command Usages: ");
        for(auto& usage : command_usage_list) {
            RK_INFO(Console, "{} : {}", usage.first, usage.second);
        }
    }

    void Console::Input(const std::string& input) {
        current_input = input;
        ParseCommand();
        Execute();
    }

    void Console::ParseCommand() {
        std::vector<std::string> arg_stack;
        SplitSingleChar(current_input, &arg_stack, ' ');
        if(arg_stack.size() == 0) {
            RK_ERROR(Console, "Invald Arguments");
            return;
        }
        command_name = arg_stack[0];

        // Special Help Command
        if(command_name == "help" || command_name == "h") {
            PrintUsage();
            return;
        }

        auto found = command_list.find(command_name);
        if(found == command_list.end()) {
            FindBestMatch();
            return;
        }
        command_type = found->second;
        arguments.clear();
        if(arg_stack.size() > 1) {
            for(int i = 1; i < arg_stack.size(); ++i) {
                arguments.push_back(arg_stack[i]);
            }
        }
        if(capacity > 0) {
            if(input_buffer.size() == capacity)
                input_buffer.erase(input_buffer.begin());
            input_buffer.push_back(current_input);
        }
    }

    void Console::FindBestMatch() {
        RK_WARN(Console, "Unknown Command: {}", command_name);
        float best_match = 0.0;
        std::string possible_name = "";
        for(auto& command : command_list) {
            auto result = StringMatch(command_name, command.first);
            if(result > best_match) {
                best_match = result;
                possible_name = command.first;
            }
        }
        if(best_match > 0.45) {
            RK_WARN(Console, "Possiable Command: {}", possible_name);
        }
    }

    void Console::Execute() {
        current_command = ConsoleCommandFactory::CreateCommand(command_type, arguments);
        if(current_command != nullptr)
            current_command->Execute();
        if(capacity > 0) {
            if(command_nuffer.size() == capacity)
                command_nuffer.erase(command_nuffer.begin());
            command_nuffer.push_back(current_command);
        }
    }
}
