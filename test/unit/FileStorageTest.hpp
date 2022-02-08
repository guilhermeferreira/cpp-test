#ifndef FILE_STORAGE_TEST_H_INCLUDED
#define FILE_STORAGE_TEST_H_INCLUDED

#include "persistence/FileStorage.hpp"

#include <stdexcept>

#include <gtest/gtest.h>

namespace test::unit {

//--------------------------------------------------------------------------

class FileStorageTestFixture : public ::testing::Test
{
public:
	FileStorageTestFixture()
	: _value1{}
	, _value2{}
	, _value3{}
	, _value4{}
	{
	}

	void SetUp() override {
		FileStorage storage{ TEST_DST_DIR "database.ini" };

		storage.load();

		// Save original values
		_value1 = storage.get("Key1");
		_value2 = storage.get("Key2");
		_value3 = storage.get("Key3");
		_value4 = storage.get("Key4");
	}

	void TearDown() override {
		FileStorage storage{ TEST_DST_DIR "database.ini" };

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

TEST_F(FileStorageTestFixture, load)
{
    FileStorage storage{ TEST_DST_DIR "database.ini" };

	// Before load(), get() throws exception
	EXPECT_THROW(storage.get("Key1"), std::runtime_error);
	EXPECT_THROW(storage.get("Key2"), std::runtime_error);
	EXPECT_THROW(storage.get("Key3"), std::runtime_error);
	EXPECT_THROW(storage.get("Key4"), std::runtime_error);

	storage.load();

	// After load(), get() retrieves the value
	EXPECT_EQ(storage.get("Key1"), "Value1");
	EXPECT_EQ(storage.get("Key2"), "Value2");
	EXPECT_EQ(storage.get("Key3"), "Value3");
	EXPECT_EQ(storage.get("Key4"), "");
}

//--------------------------------------------------------------------------

TEST_F(FileStorageTestFixture, store)
{
    FileStorage storage{ TEST_DST_DIR "database.ini" };

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

TEST_F(FileStorageTestFixture, get)
{
	FileStorage storage{ TEST_DST_DIR "database.ini" };

	storage.load();

	EXPECT_EQ(storage.get("Key1"), "Value1");
}

//--------------------------------------------------------------------------

TEST_F(FileStorageTestFixture, set)
{
	FileStorage storage{ TEST_DST_DIR "database.ini" };

	storage.set("Key1", "LongValue1");
	EXPECT_EQ(storage.get("Key1"), "LongValue1");
}

//--------------------------------------------------------------------------

TEST_F(FileStorageTestFixture, del)
{
	FileStorage storage{ TEST_DST_DIR "database.ini" };

	storage.set("Key1", "DeleteValue1");
	EXPECT_EQ(storage.get("Key1"), "DeleteValue1");
	storage.del("Key1");
	EXPECT_THROW(storage.get("Key1"), std::runtime_error);
}

//--------------------------------------------------------------------------

} // test::unit

#endif // FILE_STORAGE_TEST_H_INCLUDED
