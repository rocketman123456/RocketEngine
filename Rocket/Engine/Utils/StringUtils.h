#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

namespace Rocket {
    static void SplitMultiChar(const std::string& in, std::vector<std::string>* out, const std::string& token) {
        assert(out != nullptr);
        out->clear();
        std::string temp;
        for (int i = 0; i < int(in.size()); i++) {
            std::string test = in.substr(i, token.size());
            if (test == token) {
                if (!temp.empty()) {
                    out->push_back(temp);
                    temp.clear();
                    i += (int)token.size() - 1;
                } else {
                    out->push_back("");
                }
            } else if (i + token.size() >= in.size()) {
                temp += in.substr(i, token.size());
                out->push_back(temp);
                break;
            } else {
                temp += in[i];
            }
        }
        // Remove Empty String
        for(auto it = out->begin(); it != out->end();) {
            if((*it).length() == 0) { it = out->erase(it); } 
            else { ++it; }
        }
    }

    static void SplitSingleChar(const std::string &in, std::vector<std::string>* out, const char token) {
        assert(out != nullptr);
        out->clear();
        std::size_t start = 0;
        std::size_t end = 0;
        while ((end = in.find(token, start)) != std::string::npos) {
            out->push_back(in.substr(start, end - start));
            start = end + 1;
        }
        out->push_back(in.substr(start));
        // Remove Empty String
        for(auto it = out->begin(); it != out->end();) {
            if((*it).length() == 0) { it = out->erase(it); } 
            else { ++it; }
        }
    }

    // Split string into first and second part
    static void SplitLastSingleChar(const std::string& name, std::string* first, std::string* second, const char token) {
        assert(first != nullptr && second != nullptr);
        std::size_t found = name.rfind(token);
        if (found != std::string::npos) {
            *first = name.substr(0, found);
            *second = name.substr(found + 1, name.length() - found - 1);
        } else {
            *first = "";
            *second = name;
        }
    }

    static std::string Replace(const std::string& target, const std::string& from, const std::string& to) {
        std::string copy = target;
        std::size_t pos = 0;
        while ((pos = copy.find(from, pos)) != std::string::npos) {
            copy.replace(pos, from.length(), to);
            pos += to.length();
        }
        return copy;
    }

    static bool EndsWith(const std::string& full_string, const std::string& ending) {
        if (full_string.length() >= ending.length()) {
            return (0 == full_string.compare(full_string.length() - ending.length(), ending.length(), ending));
        }
        return false;
    }

    static bool StartsWith(const std::string& full_string, const std::string& starting) {
        if (full_string.length() >= starting.length()) {
            return (0 == full_string.compare(0, starting.length(), starting));
        }
        return false;
    }

    // Get tail of string after first token and possibly following spaces
    static std::string Tail(const std::string &in) {
        std::size_t token_start = in.find_first_not_of(" \t");
        std::size_t space_start = in.find_first_of(" \t", token_start);
        std::size_t tail_start = in.find_first_not_of(" \t", space_start);
        std::size_t tail_end = in.find_last_not_of(" \t");
        if (tail_start != std::string::npos && tail_end != std::string::npos) {
            return in.substr(tail_start, tail_end - tail_start + 1);
        }
        else if (tail_start != std::string::npos) {
            return in.substr(tail_start);
        }
        return "";
    }

    // Get first token of string
    static std::string FirstToken(const std::string &in) {
        if (!in.empty()) {
            std::size_t token_start = in.find_first_not_of(" \t");
            std::size_t token_end = in.find_first_of(" \t", token_start);
            if (token_start != std::string::npos && token_end != std::string::npos) {
                return in.substr(token_start, token_end - token_start);
            }
            else if (token_start != std::string::npos) {
                return in.substr(token_start);
            }
        }
        return "";
    }

    // Get element at given index position
    template <class T>
    const T & GetElement(const std::vector<T>& elements, std::string* index) {
        int idx = std::stoi(*index);
        if (idx < 0) idx = int(elements.size()) + idx;
        else idx--;
        return elements[idx];
    }
}