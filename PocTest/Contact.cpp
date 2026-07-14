#include "Contact.h"

Contact::Contact(int id, std::string name, std::string phone, std::string email)
    : id_(id), name_(std::move(name)), phone_(std::move(phone)), email_(std::move(email)) {}

int Contact::getId() const { return id_; }
const std::string& Contact::getName() const { return name_; }
const std::string& Contact::getPhone() const { return phone_; }
const std::string& Contact::getEmail() const { return email_; }

void Contact::setId(int id) { id_ = id; }
void Contact::setName(const std::string& value) { name_ = value; }
void Contact::setPhone(const std::string& value) { phone_ = value; }
void Contact::setEmail(const std::string& value) { email_ = value; }

std::string Contact::toJson() const {
    std::string out;
    out += "  {\n";
    out += "    \"id\": " + std::to_string(id_) + ",\n";
    out += "    \"name\": \"" + JsonParser::escape(name_) + "\",\n";
    out += "    \"phone\": \"" + JsonParser::escape(phone_) + "\",\n";
    out += "    \"email\": \"" + JsonParser::escape(email_) + "\"\n";
    out += "  }";
    return out;
}

std::string Contact::toDisplayString() const {
    return "ID: " + std::to_string(id_) +
        " | 이름: " + name_ +
        " | 전화번호: " + phone_ +
        " | 이메일: " + email_;
}

Contact Contact::fromJson(JsonParser& parser) {
    Contact c;
    parser.advance(); // '{'
    while (true) {
        parser.skipWhitespace();
        if (parser.isEnd() || parser.peek() == '}') {
            break;
        }
        std::string key = parser.parseString();
        parser.skipWhitespace();
        parser.advance(); // ':'
        parser.skipWhitespace();

        if (key == "id") {
            c.id_ = parser.parseNumber();
        }
        else if (key == "name") {
            c.name_ = parser.parseString();
        }
        else if (key == "phone") {
            c.phone_ = parser.parseString();
        }
        else if (key == "email") {
            c.email_ = parser.parseString();
        }

        parser.skipWhitespace();
        if (!parser.isEnd() && parser.peek() == ',') {
            parser.advance();
        }
    }
    parser.advance(); // '}'
    return c;
}
