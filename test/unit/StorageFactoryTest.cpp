#include "persistence/StorageFactory.hpp"

#include <filesystem>
#include <stdexcept>
#include <typeinfo>

#include <gtest/gtest.h>

#include "persistence/FileStorage.hpp"
#include "persistence/DBStorage.hpp"


namespace test::unit {

//--------------------------------------------------------------------------

class StorageFactoryTestFixture : public ::testing::Test
{
public:
	StorageFactoryTestFixture()
	: _cwd{}
	{
	}

	void SetUp() override {
		_cwd = std::filesystem::current_path();
		std::filesystem::current_path(TEST_DST_DIR);
	}

	void TearDown() override {
		std::filesystem::current_path(_cwd);
	}

private:
	std::filesystem::path _cwd;

};

//--------------------------------------------------------------------------

TEST_F(StorageFactoryTestFixture, create_absolute_file_storage)
{
	const std::string database{ TEST_DST_DIR "database.ini" };

	// Check base class
	std::shared_ptr<Storage> storage{ StorageFactory::create(database) };
	EXPECT_TRUE(storage);

	// Check correct derived class
	std::shared_ptr<FileStorage> file_storage{ std::dynamic_pointer_cast<FileStorage>(storage) };
	EXPECT_TRUE(file_storage);

	// Check wrong derived class
	std::shared_ptr<DBStorage> db_storage{ std::dynamic_pointer_cast<DBStorage>(storage) };
	EXPECT_FALSE(db_storage);
}

//--------------------------------------------------------------------------

TEST_F(StorageFactoryTestFixture, create_relative_file_storage)
{
	const std::string database{ "database.ini" };

	// Check base class
	std::shared_ptr<Storage> storage{ StorageFactory::create(database) };
	EXPECT_TRUE(storage);

	// Check correct derived class
	std::shared_ptr<FileStorage> file_storage{ std::dynamic_pointer_cast<FileStorage>(storage) };
	EXPECT_TRUE(file_storage);

	// Check wrong derived class
	std::shared_ptr<DBStorage> db_storage{ std::dynamic_pointer_cast<DBStorage>(storage) };
	EXPECT_FALSE(db_storage);
}

//--------------------------------------------------------------------------

TEST_F(StorageFactoryTestFixture, create_absolute_db_storage)
{
    const std::string database{ TEST_DST_DIR "database.db" };

	// Check base class
	std::shared_ptr<Storage> storage{ StorageFactory::create(database) };
	EXPECT_TRUE(storage);

	// Check wrong derived class
	std::shared_ptr<FileStorage> file_storage{ std::dynamic_pointer_cast<FileStorage>(storage) };
	EXPECT_FALSE(file_storage);

	// Check correct derived class
	std::shared_ptr<DBStorage> db_storage{ std::dynamic_pointer_cast<DBStorage>(storage) };
	EXPECT_TRUE(db_storage);
}

//--------------------------------------------------------------------------

TEST_F(StorageFactoryTestFixture, create_relative_db_storage)
{
    const std::string database{ "database.db" };

	// Check base class
	std::shared_ptr<Storage> storage{ StorageFactory::create(database) };
	EXPECT_TRUE(storage);

	// Check wrong derived class
	std::shared_ptr<FileStorage> file_storage{ std::dynamic_pointer_cast<FileStorage>(storage) };
	EXPECT_FALSE(file_storage);

	// Check correct derived class
	std::shared_ptr<DBStorage> db_storage{ std::dynamic_pointer_cast<DBStorage>(storage) };
	EXPECT_TRUE(db_storage);
}

//--------------------------------------------------------------------------

TEST_F(StorageFactoryTestFixture, create_failure_invalid_extension)
{
	EXPECT_THROW(StorageFactory::create(std::string(TEST_DST_DIR "database.exe")), std::runtime_error);
	EXPECT_THROW(StorageFactory::create(std::string("database.exe")), std::runtime_error);

	EXPECT_THROW(StorageFactory::create(std::string(TEST_DST_DIR "database")), std::runtime_error);
	EXPECT_THROW(StorageFactory::create(std::string("database")), std::runtime_error);

	EXPECT_THROW(StorageFactory::create(std::string(TEST_DST_DIR "database.txt")), std::runtime_error);
	EXPECT_THROW(StorageFactory::create(std::string("database.txt")), std::runtime_error);
}

//--------------------------------------------------------------------------

} // test::unit
