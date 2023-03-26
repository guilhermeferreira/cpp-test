#include "Core.hpp"

#include <map>
#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "interface/Interface.hpp"
#include "persistence/Storage.hpp"


using namespace testing;

namespace test::unit {

//--------------------------------------------------------------------------
// MockInterface - an Interface double for test purposes
//--------------------------------------------------------------------------

class MockInterface : public Interface {
public:
	MOCK_METHOD(std::string, request, (), (override));
	MOCK_METHOD(void, reply, (const std::string_view resp), (override));

};

//--------------------------------------------------------------------------
// MockStorage - a Storage double for test purposes
//--------------------------------------------------------------------------

class MockStorage : public Storage
{
public:
	MOCK_METHOD(void, load, (), (override));
	MOCK_METHOD(void, store, (), (override));
	MOCK_METHOD(std::string, get, (const std::string_view key), (override));
	MOCK_METHOD(void, set, (
		const std::string_view key,
		const std::string_view value
	), (override));
	MOCK_METHOD(void, del, (const std::string_view key), (override));

};

//--------------------------------------------------------------------------

TEST(CoreTestSuite, loop)
{
	std::shared_ptr<MockInterface> interface{ std::make_shared<MockInterface>() };
	std::shared_ptr<MockStorage> storage{ std::make_shared<MockStorage>() };
	Core core{ interface, storage };

	// Define the behavior when we call mock methods
	ON_CALL(*interface, request()).WillByDefault(Return("exit"));

	// Define what behavior we expect from these methods
	EXPECT_CALL(*interface, request()).Times(AtLeast(1));
	EXPECT_CALL(*interface, reply(Eq("OK"))).Times(AtLeast(1));
	EXPECT_CALL(*storage, load()).Times(1);
	EXPECT_CALL(*storage, store()).Times(AtLeast(1));

	core.loop();
}

//--------------------------------------------------------------------------

TEST(CoreTestSuite, command)
{
	std::shared_ptr<MockInterface> interface{ std::make_shared<MockInterface>() };
	std::shared_ptr<MockStorage> storage{ std::make_shared<MockStorage>() };
	Core core{ interface, storage };

	// Define the behavior when we call mock methods
	std::unordered_map<std::string_view, std::string> m;
	ON_CALL(*storage, set).WillByDefault([&](
		const std::string_view key,
		const std::string_view value) {
			m[key] = std::string(value);
		});
	ON_CALL(*storage, get).WillByDefault([&](const std::string_view key) {
			return m[key];
		});

	// Define what behavior we expect from these methods
	EXPECT_CALL(*storage, set(Eq("DeviceName"), Eq("TestDevice_123"))).Times(1);
	EXPECT_CALL(*storage, get(Eq("DeviceName"))).Times(1);
	EXPECT_CALL(*storage, del(Eq("DeviceName"))).Times(1);

	EXPECT_EQ(core.command("SET DeviceName TestDevice_123"), "OK");
	EXPECT_EQ(core.command("GET DeviceName"), "TestDevice_123");
	EXPECT_EQ(core.command("DELETE DeviceName"), "OK");
}

//--------------------------------------------------------------------------

TEST(CoreTestSuite, invalid_command)
{
	std::shared_ptr<MockInterface> interface{ std::make_shared<MockInterface>() };
	std::shared_ptr<MockStorage> storage{ std::make_shared<MockStorage>() };
	Core core{ interface, storage };

	// Define what behavior we expect from these methods
	EXPECT_CALL(*storage, set).Times(0);
	EXPECT_CALL(*storage, get).Times(0);
	EXPECT_CALL(*storage, del).Times(0);

	EXPECT_EQ(core.command("SE DeviceName TestDevice_123"), "> ");
	EXPECT_EQ(core.command("set DeviceName TestDevice_123"), "> ");
	EXPECT_EQ(core.command("ET DeviceName TestDevice_123"), "> ");

	EXPECT_EQ(core.command("GE DeviceName"), "> ");
	EXPECT_EQ(core.command("get DeviceName"), "> ");
	EXPECT_EQ(core.command("ET DeviceName"), "> ");

	EXPECT_EQ(core.command("DEL DeviceName"), "> ");
	EXPECT_EQ(core.command("delete DeviceName"), "> ");
	EXPECT_EQ(core.command("ELETE DeviceName"), "> ");
}

//--------------------------------------------------------------------------

TEST(CoreTestSuite, is_special_command)
{
	std::shared_ptr<MockInterface> interface{ std::make_shared<MockInterface>() };
	std::shared_ptr<MockStorage> storage{ std::make_shared<MockStorage>() };
	Core core{ interface, storage };

	EXPECT_TRUE(core.is_special_command("exit"));
	EXPECT_TRUE(core.is_special_command("store"));
}

//--------------------------------------------------------------------------

TEST(CoreTestSuite, special_command)
{
	std::shared_ptr<MockInterface> interface{ std::make_shared<MockInterface>() };
	std::shared_ptr<MockStorage> storage{ std::make_shared<MockStorage>() };
	Core core{ interface, storage };

	EXPECT_CALL(*storage, store()).Times(1);
	EXPECT_EQ(core.special_command("exit"), "OK");

	EXPECT_CALL(*storage, store()).Times(1);
	EXPECT_EQ(core.special_command("store"), "OK");
}

//--------------------------------------------------------------------------

} // test::unit
