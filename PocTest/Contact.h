#pragma once

#include "Json.h"
#include <string>

class Contact {
public:
    Contact() = default;
    Contact(int id, std::string name, std::string phone, std::string email);

    int getId() const;
    const std::string& getName() const;
    const std::string& getPhone() const;
    const std::string& getEmail() const;

    void setId(int id);
    void setName(const std::string& value);
    void setPhone(const std::string& value);
    void setEmail(const std::string& value);

    std::string toJson() const;
    std::string toDisplayString() const;

    static Contact fromJson(JsonParser& parser);

private:
    int id_ = 0;
    std::string name_;
    std::string phone_;
    std::string email_;
};
