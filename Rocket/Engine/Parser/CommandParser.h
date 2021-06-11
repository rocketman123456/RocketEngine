#pragma once
#include "Core/Core.h"

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>

namespace Rocket {
    class CommandParser {
    public:
        CommandParser(int argc, char **argv);
        ~CommandParser() = default;

        void Parse();
        std::string GetCommandData(const std::string& cmd) const;
        std::pair<std::string, std::string> GetCommand(int index) const;

    private:
        std::vector<std::string> origin_data_;
        std::unordered_map<std::string, std::string> cmd_data_map_;
        std::vector<std::string> command_;
        std::vector<std::string> data_;
    };

    extern CommandParser* g_CommandParser;
}
