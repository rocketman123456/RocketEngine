#include "Parser/JsonParser.h"
#include "FileSystem/FileSystem.h"
#include "FileSystem/OsFile.h"

#include <stdlib.h>

#ifndef JSON_PARSE_STACK_INIT_SIZE
#define JSON_PARSE_STACK_INIT_SIZE 256
#endif

#ifndef LEPT_PARSE_STRINGIFY_INIT_SIZE
#define LEPT_PARSE_STRINGIFY_INIT_SIZE 256
#endif

#define EXPECT(c, ch)       do { assert(c.json.data()[c.current] == (ch)); c.current++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')
#define PUTC(c, ch)         do { *(char*)ContextPush(c, sizeof(char)) = (ch); } while(0)

namespace Rocket {
    int32_t JsonParser::Initialize(const std::string& path, const std::string& file_name) {
        auto file = FileSystem::OpenSync(path, file_name, FileOperateMode::ReadBinary);
        FileBuffer buffer;
        file->ReadAll(buffer);
        context_.json = std::string((char*)buffer.buffer);
        context_.current = 0;
        delete[] buffer.buffer;
        FileSystem::CloseSync(std::move(file));

        int32_t result = Parser(context_, value_);
        return result;
    }

    int32_t JsonParser::Initialize(const std::string& content) {
        context_.json = content;
        context_.current = 0;

        int32_t result = Parser(context_, value_);
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
            if(c.json.data()[c.current] != '\0') {
                v.type = JSON_NULL;
                return JSON_PARSE_ROOT_NOT_SINGULAR;
            }
        }
        if(context_.stack) {
            free(context_.stack);
        }
        return 0;
    }

    int32_t JsonParser::ParseValue(JsonContext& c, JsonValue& v) {
        switch (c.json.data()[c.current]) {
            case 't':  return ParseLiteral(c, v, "true", JSON_TRUE);
            case 'f':  return ParseLiteral(c, v, "false", JSON_FALSE);
            case 'n':  return ParseLiteral(c, v, "null", JSON_NULL);
            case '\"': return ParseString(c, v);
            default:   return ParseNumber(c, v);
            case '\0': return JSON_PARSE_EXPECT_VALUE;
        }
    }

    int32_t JsonParser::ParseLiteral(JsonContext& c, JsonValue& v, const std::string& literal, JsonType type) {
        EXPECT(c, literal.data()[0]);
        int32_t i = 0;
        for(i = 0; i < literal.size() && c.current + i < c.json.size(); ++i) {
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
        const char* p = &c.json.data()[c.current];
        int32_t current = 0;
        if (p[current] == '-') current++;
        if (p[current] == '0') current++;
        else {
            if (!ISDIGIT1TO9(p[current])) return JSON_PARSE_INVALID_VALE;
            for (current++; ISDIGIT(p[current]); current++);
        }
        if (p[current] == '.') {
            current++;
            if (!ISDIGIT(p[current])) return JSON_PARSE_INVALID_VALE;
            for (current++; ISDIGIT(p[current]); current++);
        }
        if (p[current] == 'e' || p[current] == 'E') {
            current++;
            if (p[current] == '+' || p[current] == '-') current++;
            if (!ISDIGIT(p[current])) return JSON_PARSE_INVALID_VALE;
            for (current++; ISDIGIT(p[current]); current++);
        }
        errno = 0;
        v.value_d = strtod(&c.json.data()[c.current], nullptr);
        if (errno == ERANGE && (v.value_d == HUGE_VAL || v.value_d == -HUGE_VAL))
            return JSON_PARSE_NUMBER_TOO_BIG;
        c.current = c.current + current;
        v.type = JSON_NUMBER;
        return JSON_PARSE_OK;
    }

    void* JsonParser::ContextPush(JsonContext& c, int32_t size) {
        void* ret;
        assert(size > 0);
        if (c.top + size >= c.size) {
            if (c.size == 0)
                c.size = JSON_PARSE_STACK_INIT_SIZE;
            while (c.top + size >= c.size)
                c.size += c.size >> 1;  // c->size * 1.5
            c.stack = (char*)realloc(c.stack, c.size);
        }
        ret = c.stack + c.top;
        c.top += size;
        return ret;
    }

    void* JsonParser::ContextPop(JsonContext& c, int32_t size) {
        assert(c.top >= size);
        void* ret = c.stack + (c.top -= size);        
        return ret;
    }

    void JsonParser::EncodeUTF8(JsonContext& c, uint32_t u) {
        if (u <= 0x7F) 
            PUTC(c, u & 0xFF);
        else if (u <= 0x7FF) {
            PUTC(c, 0xC0 | ((u >> 6) & 0xFF));
            PUTC(c, 0x80 | ( u       & 0x3F));
        }
        else if (u <= 0xFFFF) {
            PUTC(c, 0xE0 | ((u >> 12) & 0xFF));
            PUTC(c, 0x80 | ((u >>  6) & 0x3F));
            PUTC(c, 0x80 | ( u        & 0x3F));
        }
        else {
            assert(u <= 0x10FFFF);
            PUTC(c, 0xF0 | ((u >> 18) & 0xFF));
            PUTC(c, 0x80 | ((u >> 12) & 0x3F));
            PUTC(c, 0x80 | ((u >>  6) & 0x3F));
            PUTC(c, 0x80 | ( u        & 0x3F));
        }
    }

    int32_t JsonParser::ParseHex4(const char* p, int32_t& current, uint32_t& u) {
        int i;
        u = 0;
        for (i = 0; i < 4; i++) {
            char ch = p[current];
            current++;
            u <<= 4;
            if      (ch >= '0' && ch <= '9')  u |= ch - '0';
            else if (ch >= 'A' && ch <= 'F')  u |= ch - ('A' - 10);
            else if (ch >= 'a' && ch <= 'f')  u |= ch - ('a' - 10);
            else return -1;
        }
        return current;
    }

#define STRING_ERROR(ret)   do { c.top = head; return ret; } while(0)

    int32_t JsonParser::ParseString(JsonContext& c, JsonValue& v) {
        size_t head = c.top;
        size_t len = 0;
        const char* p = nullptr;
        int32_t current = 0;
        uint32_t u, u2;
        EXPECT(c, '\"');
        v.type = JSON_STRING;
        p = &c.json.data()[c.current];
        for (;;) {
            char ch = p[current];
            current++;
            switch (ch) {
                case '\"':
                    len = c.top - head;
                    SetString(v, (const char*)ContextPop(c, len), len);
                    c.current += current;
                    return JSON_PARSE_OK;
                case '\\':
                    switch (current++) {
                        case '\"': PUTC(c, '\"'); break;
                        case '\\': PUTC(c, '\\'); break;
                        case '/':  PUTC(c, '/' ); break;
                        case 'b':  PUTC(c, '\b'); break;
                        case 'f':  PUTC(c, '\f'); break;
                        case 'n':  PUTC(c, '\n'); break;
                        case 'r':  PUTC(c, '\r'); break;
                        case 't':  PUTC(c, '\t'); break;
                        case 'u':
                            current = ParseHex4(p, current, u);
                            if (current < 0)
                                STRING_ERROR(JSON_PARSE_INVALID_UNICODE_HEX);
                            if (u >= 0xD800 && u <= 0xDBFF) { /* surrogate pair */
                                if (p[current++] != '\\')
                                    STRING_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE);
                                if (p[current++] != 'u')
                                    STRING_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE);
                                current = ParseHex4(p, current, u2);
                                if (current < 0)
                                    STRING_ERROR(JSON_PARSE_INVALID_UNICODE_HEX);
                                if (u2 < 0xDC00 || u2 > 0xDFFF)
                                    STRING_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE);
                                u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                            }
                            EncodeUTF8(c, u);
                            break;
                        default:
                            c.top = head;
                            return JSON_PARSE_INVALID_STRING_ESCAPE;
                    }
                    break;
                case '\0':
                    c.top = head;
                    return JSON_PARSE_MISS_QUOTATION_MARK;
                default:
                    if ((unsigned char)ch < 0x20) { 
                        c.top = head;
                        return JSON_PARSE_INVALID_STRING_CHAR;
                    }
                    PUTC(c, ch);
            }
        }
        return JSON_PARSE_OK;
    }

#undef STRING_ERROR

    int32_t JsonParser::ParseArray() {
        return JSON_PARSE_OK;
    }
}
