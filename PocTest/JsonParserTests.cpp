#include <gtest/gtest.h>
#include "Json.h"

TEST(JsonParserTest, ParsesSimpleString) {
    std::string text = "\"hello\"";
    JsonParser parser(text);
    EXPECT_EQ(parser.parseString(), "hello");
}

TEST(JsonParserTest, ParsesEscapedQuote) {
    std::string text = "\"he said \\\"hi\\\"\"";
    JsonParser parser(text);
    EXPECT_EQ(parser.parseString(), "he said \"hi\"");
}

TEST(JsonParserTest, ParsesNumber) {
    std::string text = "1234";
    JsonParser parser(text);
    EXPECT_EQ(parser.parseNumber(), 1234);
}

TEST(JsonParserTest, ParsesNegativeNumber) {
    std::string text = "-42";
    JsonParser parser(text);
    EXPECT_EQ(parser.parseNumber(), -42);
}

TEST(JsonParserTest, SkipsWhitespace) {
    std::string text = "   \n\t abc";
    JsonParser parser(text);
    parser.skipWhitespace();
    EXPECT_EQ(parser.peek(), 'a');
}

TEST(JsonParserTest, EscapeAddsBackslashesForQuotesAndBackslashes) {
    EXPECT_EQ(JsonParser::escape("a\"b\\c"), "a\\\"b\\\\c");
}

TEST(JsonParserTest, IsEndReflectsPosition) {
    std::string text = "1";
    JsonParser parser(text);
    EXPECT_FALSE(parser.isEnd());
    parser.parseNumber();
    EXPECT_TRUE(parser.isEnd());
}
