#pragma once

#include <string>

class JsonParser {
public:
    explicit JsonParser(const std::string& text);

    void skipWhitespace();
    bool isEnd() const;
    char peek() const;
    void advance();

    std::string parseString();
    int parseNumber();

    static std::string escape(const std::string& s);

private:
    const std::string& text_;
    size_t pos_;
};
