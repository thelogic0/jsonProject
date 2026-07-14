#include "ConsoleApp.h"
#include <iostream>

ConsoleApp::ConsoleApp(const std::string& dataFile) : repository_(dataFile) {}

std::string ConsoleApp::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void ConsoleApp::printMenu() const {
    std::cout << "\n===== 연락처 관리 (JSON CRUD) =====" << std::endl;
    std::cout << "1. Create - 데이터 추가" << std::endl;
    std::cout << "2. Read   - 전체 목록 보기" << std::endl;
    std::cout << "3. Read   - ID/이름으로 검색" << std::endl;
    std::cout << "4. Update - 데이터 수정" << std::endl;
    std::cout << "5. Delete - 데이터 삭제" << std::endl;
    std::cout << "0. 종료" << std::endl;
    std::cout << "선택: ";
}

void ConsoleApp::runCreate() {
    int id = repository_.generateNextId();
    std::string name = readLine("이름: ");
    std::string phone = readLine("전화번호: ");
    std::string email = readLine("이메일: ");

    repository_.add(Contact(id, name, phone, email));
    std::cout << "생성 완료 (ID: " << id << ")" << std::endl;
}

void ConsoleApp::runList() const {
    const auto& contacts = repository_.getAll();
    if (contacts.empty()) {
        std::cout << "저장된 데이터가 없습니다." << std::endl;
        return;
    }
    for (const auto& c : contacts) {
        std::cout << c.toDisplayString() << std::endl;
    }
}

void ConsoleApp::runSearch() const {
    std::string key = readLine("검색할 ID 또는 이름: ");
    std::vector<Contact> results = repository_.search(key);

    if (results.empty()) {
        std::cout << "일치하는 데이터가 없습니다." << std::endl;
        return;
    }
    for (const auto& c : results) {
        std::cout << c.toDisplayString() << std::endl;
    }
}

void ConsoleApp::runUpdate() {
    std::string idStr = readLine("수정할 데이터의 ID: ");
    int id;
    try {
        id = std::stoi(idStr);
    }
    catch (...) {
        std::cout << "잘못된 ID 입니다." << std::endl;
        return;
    }

    if (!repository_.findById(id)) {
        std::cout << "해당 ID의 데이터를 찾을 수 없습니다." << std::endl;
        return;
    }

    std::string choice = readLine("수정할 필드를 선택하세요 (1: 이름, 2: 전화번호, 3: 이메일): ");
    bool ok = false;

    if (choice == "1") {
        ok = repository_.updateName(id, readLine("새 이름: "));
    }
    else if (choice == "2") {
        ok = repository_.updatePhone(id, readLine("새 전화번호: "));
    }
    else if (choice == "3") {
        ok = repository_.updateEmail(id, readLine("새 이메일: "));
    }
    else {
        std::cout << "잘못된 선택입니다." << std::endl;
        return;
    }

    std::cout << (ok ? "수정 완료" : "수정 실패") << std::endl;
}

void ConsoleApp::runDelete() {
    std::string idStr = readLine("삭제할 데이터의 ID: ");
    int id;
    try {
        id = std::stoi(idStr);
    }
    catch (...) {
        std::cout << "잘못된 ID 입니다." << std::endl;
        return;
    }

    Contact* target = repository_.findById(id);
    if (!target) {
        std::cout << "해당 ID의 데이터를 찾을 수 없습니다." << std::endl;
        return;
    }

    std::cout << "다음 데이터를 삭제하시겠습니까? (y/n)" << std::endl;
    std::cout << target->toDisplayString() << std::endl;
    std::string confirm = readLine("");

    if (confirm == "y" || confirm == "Y") {
        repository_.remove(id);
        std::cout << "삭제 완료" << std::endl;
    }
    else {
        std::cout << "삭제가 취소되었습니다." << std::endl;
    }
}

void ConsoleApp::run() {
    while (true) {
        printMenu();
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            runCreate();
        }
        else if (choice == "2") {
            runList();
        }
        else if (choice == "3") {
            runSearch();
        }
        else if (choice == "4") {
            runUpdate();
        }
        else if (choice == "5") {
            runDelete();
        }
        else if (choice == "0") {
            std::cout << "프로그램을 종료합니다." << std::endl;
            break;
        }
        else {
            std::cout << "잘못된 선택입니다. 다시 입력해주세요." << std::endl;
        }
    }
}
