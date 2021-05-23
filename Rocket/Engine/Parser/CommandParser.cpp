#include "Parser/CommandParser.h"

namespace Rocket {
    static Vec<String> Split(const String& str, const String& pattern) {
        Vec<String> res;
        if(str == "")
            return res;

        // For Final Part
        String strs = str + pattern;
        size_t pos = strs.find(pattern);

        while(pos != strs.npos) {
            String temp = strs.substr(0, pos);
            res.push_back(temp);
            // Remove Current Part
            strs = strs.substr(pos+1, strs.size());
            pos = strs.find(pattern);
        }

        return res;
    }

    CommandParser::CommandParser(int argc, char **argv) {
        for(int i = 0; i < argc; ++i) {
            origin_data_.emplace_back(argv[i]);
        }
    }

    void CommandParser::Parse() {
        for(auto cmd : origin_data_) {
            auto result = Split(cmd, "=");
            if(result.size() == 1) {
                RK_CORE_INFO("Command with no data: cmd {}", result[0]);
                command_.push_back(result[0]);
                data_.push_back("");
                cmd_data_map_[result[0]] = "";
            }
            else if(result.size() == 2) {
                RK_CORE_INFO("Command with data: cmd {}, data {}", result[0], result[1]);
                command_.push_back(result[0]);
                data_.push_back(result[1]);
                cmd_data_map_[result[0]] = result[1];
            }
            else {
                RK_CORE_ERROR("Command with too many data");
            }
        }
    }

    String CommandParser::GetCommandData(const String& cmd) const {
        auto it = cmd_data_map_.find(cmd);
        if(it == cmd_data_map_.end()) {
            return "";
        }
        else {
            return it->second;
        }
    }

    std::pair<String, String> CommandParser::GetCommand(int index) const {
        RK_CORE_ASSERT(index < command_.size(), "Invalid Command Index");
        RK_CORE_ASSERT(index < data_.size(), "Invalid Command Index");
        return {command_[index], data_[index]};
    }
}
