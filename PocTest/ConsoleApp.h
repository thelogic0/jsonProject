#pragma once

#include "ContactRepository.h"
#include <string>

class ConsoleApp {
public:
    explicit ConsoleApp(const std::string& dataFile);

    void run();

private:
    void printMenu() const;
    static std::string readLine(const std::string& prompt);

    void runCreate();
    void runList() const;
    void runSearch() const;
    void runUpdate();
    void runDelete();

    ContactRepository repository_;
};
