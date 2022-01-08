#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <climits>
#include <fstream>
#include <sstream>
#include <iomanip>

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
        //assert(first != nullptr && second != nullptr);
        std::size_t found = name.rfind(token);
        if (found != std::string::npos) {
            if(first != nullptr)
                *first = name.substr(0, found);
            if(second != nullptr)
                *second = name.substr(found + 1, name.length() - found - 1);
        } else {
            if(first != nullptr)
                *first = "";
            if(second != nullptr)
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
#if 0
        if (full_string.length() >= ending.length()) {
            return (0 == full_string.compare(full_string.length() - ending.length(), ending.length(), ending));
        }
        return false;
#else
        return (strstr(full_string.c_str(), ending.c_str()) - full_string.c_str()) == (full_string.length() - ending.length());
#endif
    }

    static bool StartsWith(const std::string& full_string, const std::string& starting) {
        if (full_string.length() >= starting.length()) {
            return (0 == full_string.compare(0, starting.length(), starting));
        }
        return false;
    }

    // convert a type to a string
    template <class T>
    inline std::string TypeToStr(const T& t, int precision = 2) {
        std::ostringstream buffer;
        buffer << std::fixed << std::setprecision(precision) << t;
        return buffer.str();
    }

    // convert a bool to a string
    static inline std::string BoolToStr(bool b) {
        if(b) return "true";
        return "false";
    }

    // grabs a value of the specified type from an input stream
    template <typename T>
    inline T GetValueFromStream(std::ifstream& stream) {
        T val;
        stream >> val;
        //make sure it was the correct type
        if (!stream) {
            throw std::runtime_error("Attempting to retrieve wrong type from stream");
        }
        return val;
    }

    static float StringMatch(const std::string& left, const std::string& right) {
        std::size_t left_size = left.length();
        std::size_t right_size = right.length();
        std::size_t larger_size = left_size > right_size ? left_size : right_size;
        std::size_t left_ptr = 0;
        std::size_t right_ptr = 0;
        const float CAP_MISMATCH_VAL = 0.9f;
        float match_val = 0.0f;

        while(left_ptr < left_size && right_ptr < right_size) {
            if(left.at(left_ptr) == right.at(right_ptr)) {
                match_val += 1.0f / (float)larger_size;
                if(left_ptr < left_size)
                    ++left_ptr;
                if(right_ptr < right_size)
                    ++right_ptr;
            } else if(std::tolower(left.at(left_ptr)) == std::tolower(right.at(right_ptr))) {
                match_val += CAP_MISMATCH_VAL / (float)larger_size;
                if(left_ptr < left_size)
                    ++left_ptr;
                if(right_ptr < right_size)
                    ++right_ptr;
            } else {
                std::size_t lpbest = left_size;
                std::size_t rpbest = right_size;
                std::size_t total_count = 0;
                std::size_t best_count = INT_MAX;
                std::size_t left_count = 0;
                std::size_t right_count = 0;
                for(auto lp = left_ptr; (lp < left_size) && ((left_count + right_count) < best_count); ++lp) {
                    for(auto rp = right_ptr; (rp < right_size) && ((left_count + right_count) < best_count); ++rp) {
                        if(std::tolower(left.at(lp)) == std::tolower(right.at(rp))) {
                            total_count = left_count + right_count;
                            if(total_count < best_count) {
                                best_count = total_count;
                                lpbest = lp;
                                rpbest = rp;
                            }
                        }
                        ++right_count;
                    }
                    ++left_count;
                    right_count = 0;
                }
                left_ptr = lpbest;
                right_ptr = rpbest;
            }
        }
        if(match_val > 0.999) {
            match_val = 1.0f;
        } else if(match_val < 0.001) {
            match_val = 0.0f;
        }
        return match_val;
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
    const T & GetElement(const std::vector<T>& elements, std::string& index) {
        int idx = std::stoi(index);
        if (idx < 0) idx = int(elements.size()) + idx;
        else idx--;
        return elements[idx];
    }
}