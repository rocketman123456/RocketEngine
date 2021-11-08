#pragma once
#include <string>
#include <vector>

namespace Rocket {
    namespace Algorithm {
        // Split a String into a string array at a given token
        void Split(const std::string &in, std::vector<std::string> &out, std::string token);

        // Get tail of string after first token and possibly following spaces
        std::string Tail(const std::string &in);

        // Get first token of string
        std::string FirstToken(const std::string &in);

        // Get element at given index position
        template <class T>
        const T & GetElement(const std::vector<T> &elements, std::string &index) {
            int idx = std::stoi(index);
            if (idx < 0) idx = int(elements.size()) + idx;
            else idx--;
            return elements[idx];
        }
    }
}