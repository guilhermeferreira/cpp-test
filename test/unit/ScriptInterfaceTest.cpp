#include "interface/ScriptInterface.hpp"

#include <gtest/gtest.h>

namespace test::unit {

//--------------------------------------------------------------------------

TEST(ScriptInterfaceTestSuite, open_non_existent)
{
	EXPECT_THROW(new ScriptInterface{ TEST_DST_DIR "non_existent.txt" }, std::runtime_error);
}

//--------------------------------------------------------------------------

TEST(ScriptInterfaceTestSuite, open_directory)
{
	EXPECT_THROW(new ScriptInterface{ TEST_DST_DIR }, std::runtime_error);
}

//--------------------------------------------------------------------------

TEST(ScriptInterfaceTestSuite, request)
{
	ScriptInterface script{ TEST_DST_DIR "script.txt" };

	EXPECT_EQ(script.request(), "GET Key3");
	EXPECT_EQ(script.request(), "GET Key1");
	EXPECT_EQ(script.request(), "GET Key2");
	EXPECT_EQ(script.request(), "GET Key4");
	EXPECT_EQ(script.request(), "exit");
}

//--------------------------------------------------------------------------

TEST(ScriptInterfaceTestSuite, reply)
{
	// NOTE: test must fail until it gets implemented
	EXPECT_TRUE(false);
}

//--------------------------------------------------------------------------

} // test::unit
