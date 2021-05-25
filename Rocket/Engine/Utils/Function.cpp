#include "Utils/Function.h"

namespace Rocket {
    Vec<String> Split(const String& str, const String& pattern) {
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
}