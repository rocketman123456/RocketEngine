#pragma once
#include "Core/Core.h"
#include "Core/Template.h"

#include <utility>

namespace Rocket {
    class CommandParser {
    public:
        CommandParser(int argc, char **argv);
        ~CommandParser() = default;

        void Parse();
        String GetCommandData(const String& cmd) const;
        std::pair<String, String> GetCommand(int index) const;

    private:
        Vec<String> origin_data_;
        Map<String, String> cmd_data_map_;
        Vec<String> command_;
        Vec<String> data_;
    };

    extern CommandParser* g_CommandParser;
}
