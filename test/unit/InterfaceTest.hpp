#ifndef INTERFACE_TEST_H_INCLUDED
#define INTERFACE_TEST_H_INCLUDED

#include "interface/Interface.hpp"

#include <gtest/gtest.h>

namespace test::unit {

//--------------------------------------------------------------------------

TEST(InterfaceTestSuite, parse_set)
{
	std::string cmd;
	std::string key;
	std::string value;
	std::tie(cmd, key, value) = Interface::parse("SET Key Value");
	EXPECT_EQ("SET", cmd);
	EXPECT_EQ("Key", key);
	EXPECT_EQ("Value", value);
}

//--------------------------------------------------------------------------

TEST(InterfaceTestSuite, parse_get)
{
	auto line = Interface::parse("GET Key");
	EXPECT_EQ("GET", std::get<0>(line));
	EXPECT_EQ("Key", std::get<1>(line));
	EXPECT_EQ("", std::get<2>(line));
}

//--------------------------------------------------------------------------

TEST(InterfaceTestSuite, parse_delete)
{
	auto line = Interface::parse("DELETE Key");
	EXPECT_EQ("DELETE", std::get<0>(line));
	EXPECT_EQ("Key", std::get<1>(line));
	EXPECT_EQ("", std::get<2>(line));
}

//--------------------------------------------------------------------------

TEST(InterfaceTestSuite, parse_set_without_key)
{
	EXPECT_THROW(Interface::parse("SET"), std::runtime_error);
}

//--------------------------------------------------------------------------

TEST(InterfaceTestSuite, parse_get_without_key)
{
	EXPECT_THROW(Interface::parse("GET"), std::runtime_error);
}

//--------------------------------------------------------------------------

TEST(InterfaceTestSuite, parse_delete_without_key)
{
	EXPECT_THROW(Interface::parse("DELETE"), std::runtime_error);
}

//--------------------------------------------------------------------------

TEST(InterfaceTestSuite, parse_set_with_whitespace)
{
	std::string cmd;
	std::string key;
	std::string value;
	std::tie(cmd, key, value) = Interface::parse("SET Key 'A Value with white space'");
	EXPECT_EQ("SET", cmd);
	EXPECT_EQ("Key", key);
	EXPECT_EQ("'A Value with white space'", value);
}

//--------------------------------------------------------------------------

} // test::unit

#endif // INTERFACE_TEST_H_INCLUDED
