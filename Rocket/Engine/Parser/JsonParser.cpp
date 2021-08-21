#include "Parser/JsonParser.h"
#include "FileSystem/FileSystem.h"
#include "FileSystem/OsFile.h"

#include <stdlib.h>

#define EXPECT(c, ch)       do { assert(c.json.data()[c.current] == (ch)); c.current++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')

namespace Rocket {
    int32_t JsonParser::Initialize(const std::string& path, const std::string& file_name) {
        auto file = FileSystem::OpenSync(path, file_name, FileOperateMode::ReadBinary);
        FileBuffer buffer;
        file->ReadAll(buffer);
        content_.json = std::string((char*)buffer.buffer);
        content_.current = 0;
        delete[] buffer.buffer;
        FileSystem::CloseSync(std::move(file));

        int32_t result = Parser(content_, value_);
        return result;
    }

    int32_t JsonParser::Initialize(const std::string& content) {
        content_.json = content;
        content_.current = 0;

        int32_t result = Parser(content_, value_);
        return result;
    }

    void JsonParser::Finalize() {
    }

    int32_t JsonParser::Parser(JsonContext& c, JsonValue& v) {
        int32_t result = JSON_PARSE_OK;
        v.type = JSON_NULL;
        ParseWhiteSpace(c);
        if((result = ParseValue(c, v)) == JSON_PARSE_OK) {
            ParseWhiteSpace(c);
            if(c.json.data()[c.current]) {
                return JSON_PARSE_ROOT_NOT_SINGULAR;
            }
        }
        return 0;
    }

    int32_t JsonParser::ParseLiteral(JsonContext& c, JsonValue& v, const char* literal, JsonType type) {
        EXPECT(c, literal[0]);
        int32_t i = 0;
        for(i = 0; literal[i+1]; ++i) {
            if(c.json.data()[c.current + i] != literal[i]) {
                return JSON_PARSE_INVALID_VALE;
            }
        }
        c.current += i;
        v.type = type;
        return JSON_PARSE_OK;
    }

    void JsonParser::ParseWhiteSpace(JsonContext& c) {
        while(c.current < c.json.size() && (
            c.json.data()[c.current] == ' ' || 
            c.json.data()[c.current] == '\t' || 
            c.json.data()[c.current] == '\n' ||
            c.json.data()[c.current] == '\r')) {
            c.current++;
        }
    }

    int32_t JsonParser::ParseNumber(JsonContext& c, JsonValue& v) {
        int32_t end = 0;
        // TODO : parse number
        v.value = strtod(&c.json.data()[c.current], nullptr);
        if (c.json.size() == end)
            return JSON_PARSE_INVALID_VALE;
        c.current = end;
        v.type = JSON_NUMBER;
        return JSON_PARSE_OK;
    }

    int32_t JsonParser::ParseValue(JsonContext& c, JsonValue& v) {
        switch (c.json.data()[c.current]) {
            case 't':  return ParseLiteral(c, v, "true", JSON_TRUE);
            case 'f':  return ParseLiteral(c, v, "false", JSON_FALSE);
            case 'n':  return ParseLiteral(c, v, "null", JSON_NULL);
            default:   return ParseNumber(c, v);
            case '\0': return JSON_PARSE_EXPECT_VALUE;
        }
    }
}
