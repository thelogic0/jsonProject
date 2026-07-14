#include "ContactRepository.h"
#include "Json.h"
#include <algorithm>
#include <fstream>
#include <sstream>

ContactRepository::ContactRepository(std::string filePath) : filePath_(std::move(filePath)) {
    load();
}

void ContactRepository::load() {
    std::ifstream in(filePath_);
    if (!in.is_open()) {
        return;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string text = buffer.str();

    JsonParser parser(text);
    parser.skipWhitespace();
    if (parser.isEnd() || parser.peek() != '[') {
        return;
    }
    parser.advance(); // '['

    while (true) {
        parser.skipWhitespace();
        if (parser.isEnd() || parser.peek() == ']') {
            break;
        }
        if (parser.peek() == '{') {
            contacts_.push_back(Contact::fromJson(parser));
        }
        parser.skipWhitespace();
        if (!parser.isEnd() && parser.peek() == ',') {
            parser.advance();
        }
    }
}

void ContactRepository::save() const {
    std::ofstream out(filePath_);
    out << "[\n";
    for (size_t i = 0; i < contacts_.size(); ++i) {
        out << contacts_[i].toJson();
        out << (i + 1 < contacts_.size() ? ",\n" : "\n");
    }
    out << "]\n";
}

const std::vector<Contact>& ContactRepository::getAll() const {
    return contacts_;
}

int ContactRepository::generateNextId() const {
    int maxId = 0;
    for (const auto& c : contacts_) {
        maxId = std::max(maxId, c.getId());
    }
    return maxId + 1;
}

Contact* ContactRepository::findById(int id) {
    auto it = std::find_if(contacts_.begin(), contacts_.end(),
        [id](const Contact& c) { return c.getId() == id; });
    return it == contacts_.end() ? nullptr : &(*it);
}

std::vector<Contact> ContactRepository::search(const std::string& key) const {
    std::vector<Contact> results;
    for (const auto& c : contacts_) {
        if (std::to_string(c.getId()) == key || c.getName() == key) {
            results.push_back(c);
        }
    }
    return results;
}

void ContactRepository::add(const Contact& contact) {
    contacts_.push_back(contact);
    save();
}

bool ContactRepository::remove(int id) {
    auto it = std::find_if(contacts_.begin(), contacts_.end(),
        [id](const Contact& c) { return c.getId() == id; });
    if (it == contacts_.end()) {
        return false;
    }
    contacts_.erase(it);
    save();
    return true;
}

bool ContactRepository::updateName(int id, const std::string& value) {
    Contact* c = findById(id);
    if (!c) {
        return false;
    }
    c->setName(value);
    save();
    return true;
}

bool ContactRepository::updatePhone(int id, const std::string& value) {
    Contact* c = findById(id);
    if (!c) {
        return false;
    }
    c->setPhone(value);
    save();
    return true;
}

bool ContactRepository::updateEmail(int id, const std::string& value) {
    Contact* c = findById(id);
    if (!c) {
        return false;
    }
    c->setEmail(value);
    save();
    return true;
}
