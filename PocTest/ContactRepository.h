#pragma once

#include "Contact.h"
#include <string>
#include <vector>

class ContactRepository {
public:
    explicit ContactRepository(std::string filePath);

    const std::vector<Contact>& getAll() const;
    int generateNextId() const;
    Contact* findById(int id);
    std::vector<Contact> search(const std::string& key) const;

    void add(const Contact& contact);
    bool remove(int id);

    bool updateName(int id, const std::string& value);
    bool updatePhone(int id, const std::string& value);
    bool updateEmail(int id, const std::string& value);

    void save() const;

private:
    void load();

    std::string filePath_;
    std::vector<Contact> contacts_;
};
