#include <gtest/gtest.h>
#include "Contact.h"
#include "Json.h"

TEST(ContactTest, GettersReturnConstructedValues) {
    Contact c(1, "Hong", "010-1111-2222", "hong@test.com");
    EXPECT_EQ(c.getId(), 1);
    EXPECT_EQ(c.getName(), "Hong");
    EXPECT_EQ(c.getPhone(), "010-1111-2222");
    EXPECT_EQ(c.getEmail(), "hong@test.com");
}

TEST(ContactTest, SettersUpdateFields) {
    Contact c;
    c.setId(5);
    c.setName("Kim");
    c.setPhone("010-0000-0000");
    c.setEmail("kim@test.com");

    EXPECT_EQ(c.getId(), 5);
    EXPECT_EQ(c.getName(), "Kim");
    EXPECT_EQ(c.getPhone(), "010-0000-0000");
    EXPECT_EQ(c.getEmail(), "kim@test.com");
}

TEST(ContactTest, ToJsonAndFromJsonRoundTrip) {
    Contact c(3, "Park", "010-3333-3333", "park@test.com");
    std::string json = c.toJson();

    JsonParser parser(json);
    parser.skipWhitespace();
    Contact parsed = Contact::fromJson(parser);

    EXPECT_EQ(parsed.getId(), 3);
    EXPECT_EQ(parsed.getName(), "Park");
    EXPECT_EQ(parsed.getPhone(), "010-3333-3333");
    EXPECT_EQ(parsed.getEmail(), "park@test.com");
}

TEST(ContactTest, ToJsonEscapesSpecialCharacters) {
    Contact c(1, "He said \"hi\"", "", "");
    std::string json = c.toJson();
    EXPECT_NE(json.find("\\\"hi\\\""), std::string::npos);
}

TEST(ContactTest, ToDisplayStringContainsAllFields) {
    Contact c(1, "Hong", "010-1111-2222", "hong@test.com");
    std::string display = c.toDisplayString();

    EXPECT_NE(display.find("Hong"), std::string::npos);
    EXPECT_NE(display.find("010-1111-2222"), std::string::npos);
    EXPECT_NE(display.find("hong@test.com"), std::string::npos);
}
