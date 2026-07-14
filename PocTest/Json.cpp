#include "Json.h"
#include <cctype>

JsonParser::JsonParser(const std::string& text) : text_(text), pos_(0) {}

void JsonParser::skipWhitespace() {
    while (pos_ < text_.size() && std::isspace(static_cast<unsigned char>(text_[pos_]))) {
        ++pos_;
    }
}

bool JsonParser::isEnd() const {
    return pos_ >= text_.size();
}

char JsonParser::peek() const {
    return text_[pos_];
}

void JsonParser::advance() {
    ++pos_;
}

std::string JsonParser::parseString() {
    std::string result;
    advance(); // opening quote
    while (!isEnd() && peek() != '"') {
        if (peek() == '\\' && pos_ + 1 < text_.size()) {
            advance();
            result += peek();
        }
        else {
            result += peek();
        }
        advance();
    }
    advance(); // closing quote
    return result;
}

int JsonParser::parseNumber() {
    size_t start = pos_;
    while (!isEnd() && (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '-')) {
        advance();
    }
    return std::stoi(text_.substr(start, pos_ - start));
}

std::string JsonParser::escape(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"' || c == '\\') {
            out += '\\';
        }
        out += c;
    }
    return out;
}
