#include "Utils/Function.h"

namespace Rocket {
    std::vector<std::string> Split(const std::string& str, const std::string& pattern) {
        std::vector<std::string> res;
        if(str == "")
            return res;

        // For Final Part
        std::string strs = str + pattern;
        size_t pos = strs.find(pattern);

        while(pos != strs.npos) {
            std::string temp = strs.substr(0, pos);
            res.push_back(temp);
            // Remove Current Part
            strs = strs.substr(pos+1, strs.size());
            pos = strs.find(pattern);
        }

        return std::move(res);
    }
}