#include "persistence/DBStorage.hpp"

#include <stdexcept>
#include <filesystem>

#include <gtest/gtest.h>

namespace test::unit {

//--------------------------------------------------------------------------

class DBStorageTestFixture : public ::testing::Test
{
public:
	DBStorageTestFixture()
	: _value1{}
	, _value2{}
	, _value3{}
	, _value4{}
	{
	}

	void SetUp() override {
		DBStorage storage{ TEST_DST_DIR "database.db" };

		storage.load();

		// Save original values
		_value1 = storage.get("Key1");
		_value2 = storage.get("Key2");
		_value3 = storage.get("Key3");
		_value4 = storage.get("Key4");
	}

	void TearDown() override {
		DBStorage storage{ TEST_DST_DIR "database.db" };

		// Restore original values
		storage.set("Key1", _value1);
		storage.set("Key2", _value2);
		storage.set("Key3", _value3);
		storage.set("Key4", _value4);

		storage.store();
	}

private:
	std::string _value1;
	std::string _value2;
	std::string _value3;
	std::string _value4;

};

//--------------------------------------------------------------------------

TEST_F(DBStorageTestFixture, load_existent_database)
{
    DBStorage storage{ TEST_DST_DIR "database.db" };

	storage.load();

	// After load(), get() retrieves the value
	EXPECT_EQ(storage.get("Key1"), "Value1");
	EXPECT_EQ(storage.get("Key2"), "Value2");
	EXPECT_EQ(storage.get("Key3"), "Value3");
	EXPECT_EQ(storage.get("Key4"), "");
}

//--------------------------------------------------------------------------

TEST_F(DBStorageTestFixture, load_nonexistent_database)
{
	std::filesystem::remove(TEST_DST_DIR "nonexistent.db");
    DBStorage storage{ TEST_DST_DIR "nonexistent.db" };

	// Before load(), get() throws exception
	EXPECT_THROW(storage.get("Key1"), std::runtime_error);
	EXPECT_THROW(storage.get("Key2"), std::runtime_error);
	EXPECT_THROW(storage.get("Key3"), std::runtime_error);
	EXPECT_THROW(storage.get("Key4"), std::runtime_error);

	storage.load();

	// After load(), get() still throws exception because the database did not exist
	EXPECT_THROW(storage.get("Key1"), std::runtime_error);
	EXPECT_THROW(storage.get("Key2"), std::runtime_error);
	EXPECT_THROW(storage.get("Key3"), std::runtime_error);
	EXPECT_THROW(storage.get("Key4"), std::runtime_error);

	// Create entries in the newly create database
	storage.set("Key1", "Val1");
	storage.set("Key2", "Val2");
	storage.set("Key3", "Val3");
	storage.set("Key4", "Val4");

	storage.store();

	// After set() and store(), get() retrieves the values
	EXPECT_EQ(storage.get("Key1"), "Val1");
	EXPECT_EQ(storage.get("Key2"), "Val2");
	EXPECT_EQ(storage.get("Key3"), "Val3");
	EXPECT_EQ(storage.get("Key4"), "Val4");
}

//--------------------------------------------------------------------------

TEST_F(DBStorageTestFixture, store)
{
    DBStorage storage{ TEST_DST_DIR "database.db" };

	storage.set("Key1", "Val1");
	storage.set("Key2", "Val2");
	storage.set("Key3", "Val3");
	storage.set("Key4", "Val4");

	storage.store();

	EXPECT_EQ(storage.get("Key1"), "Val1");
	EXPECT_EQ(storage.get("Key2"), "Val2");
	EXPECT_EQ(storage.get("Key3"), "Val3");
	EXPECT_EQ(storage.get("Key4"), "Val4");
}

//--------------------------------------------------------------------------

TEST_F(DBStorageTestFixture, get)
{
	DBStorage storage{ TEST_DST_DIR "database.db" };

	EXPECT_EQ(storage.get("Key1"), "Value1");
	EXPECT_EQ(storage.get("Key2"), "Value2");
	EXPECT_EQ(storage.get("Key3"), "Value3");
	EXPECT_EQ(storage.get("Key4"), "");
}

//--------------------------------------------------------------------------

TEST_F(DBStorageTestFixture, set)
{
	DBStorage storage{ TEST_DST_DIR "database.db" };

	storage.set("Key1", "LongValue1");
	EXPECT_EQ(storage.get("Key1"), "LongValue1");
}

//--------------------------------------------------------------------------

TEST_F(DBStorageTestFixture, del)
{
	DBStorage storage{ TEST_DST_DIR "database.db" };

	storage.set("Key1", "DeleteValue1");
	EXPECT_EQ(storage.get("Key1"), "DeleteValue1");
	storage.del("Key1");
	EXPECT_THROW(storage.get("Key1"), std::runtime_error);
}

//--------------------------------------------------------------------------

} // test::unit
