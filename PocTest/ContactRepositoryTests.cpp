#include <gtest/gtest.h>
#include "ContactRepository.h"
#include <cstdio>

namespace {

    const char* TEST_FILE = "test_contacts.json";

    class ContactRepositoryTest : public ::testing::Test {
    protected:
        void SetUp() override {
            std::remove(TEST_FILE);
        }
        void TearDown() override {
            std::remove(TEST_FILE);
        }
    };

}

TEST_F(ContactRepositoryTest, StartsEmptyWhenFileMissing) {
    ContactRepository repo(TEST_FILE);
    EXPECT_TRUE(repo.getAll().empty());
}

TEST_F(ContactRepositoryTest, AddPersistsContact) {
    ContactRepository repo(TEST_FILE);
    repo.add(Contact(repo.generateNextId(), "Hong", "010-1111-2222", "hong@test.com"));

    ASSERT_EQ(repo.getAll().size(), 1u);
    EXPECT_EQ(repo.getAll()[0].getName(), "Hong");
}

TEST_F(ContactRepositoryTest, ReloadsPersistedData) {
    {
        ContactRepository repo(TEST_FILE);
        repo.add(Contact(repo.generateNextId(), "Hong", "010-1111-2222", "hong@test.com"));
    }

    ContactRepository reloaded(TEST_FILE);
    ASSERT_EQ(reloaded.getAll().size(), 1u);
    EXPECT_EQ(reloaded.getAll()[0].getName(), "Hong");
}

TEST_F(ContactRepositoryTest, GenerateNextIdIncrements) {
    ContactRepository repo(TEST_FILE);
    EXPECT_EQ(repo.generateNextId(), 1);

    repo.add(Contact(1, "A", "", ""));
    EXPECT_EQ(repo.generateNextId(), 2);

    repo.add(Contact(5, "B", "", ""));
    EXPECT_EQ(repo.generateNextId(), 6);
}

TEST_F(ContactRepositoryTest, FindByIdReturnsPointerOrNull) {
    ContactRepository repo(TEST_FILE);
    repo.add(Contact(1, "Hong", "", ""));

    EXPECT_NE(repo.findById(1), nullptr);
    EXPECT_EQ(repo.findById(999), nullptr);
}

TEST_F(ContactRepositoryTest, SearchMatchesIdOrName) {
    ContactRepository repo(TEST_FILE);
    repo.add(Contact(1, "Hong", "", ""));
    repo.add(Contact(2, "Kim", "", ""));

    EXPECT_EQ(repo.search("1").size(), 1u);
    EXPECT_EQ(repo.search("Kim").size(), 1u);
    EXPECT_EQ(repo.search("nope").size(), 0u);
}

TEST_F(ContactRepositoryTest, UpdateFieldsMutateAndPersist) {
    ContactRepository repo(TEST_FILE);
    repo.add(Contact(1, "Hong", "010-1111-2222", "hong@test.com"));

    EXPECT_TRUE(repo.updateName(1, "Hong2"));
    EXPECT_TRUE(repo.updatePhone(1, "010-9999-9999"));
    EXPECT_TRUE(repo.updateEmail(1, "new@test.com"));
    EXPECT_FALSE(repo.updateName(999, "Nobody"));

    Contact* c = repo.findById(1);
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->getName(), "Hong2");
    EXPECT_EQ(c->getPhone(), "010-9999-9999");
    EXPECT_EQ(c->getEmail(), "new@test.com");
}

TEST_F(ContactRepositoryTest, RemoveDeletesContact) {
    ContactRepository repo(TEST_FILE);
    repo.add(Contact(1, "Hong", "", ""));

    EXPECT_TRUE(repo.remove(1));
    EXPECT_EQ(repo.getAll().size(), 0u);
    EXPECT_FALSE(repo.remove(1));
}
