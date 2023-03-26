#include "persistence/IniFormat.hpp"

#include <sstream>

#include <gtest/gtest.h>

namespace test::unit {

//--------------------------------------------------------------------------

TEST(IniFormatTestSuite, load)
{
	std::stringstream contents;
	contents << "Key1=Value1\n";
	contents << "Key2=Value2\n";
	contents << "Key3=Value3\n";
	contents << "Key4=Value4\n";

	IniFormat format;
	std::map<std::string, std::string> settings = format.load(contents);

	EXPECT_FALSE(settings.empty());
	EXPECT_EQ(settings.size(), 4);
	EXPECT_EQ(settings["Key1"], "Value1");
	EXPECT_EQ(settings["Key2"], "Value2");
	EXPECT_EQ(settings["Key3"], "Value3");
	EXPECT_EQ(settings["Key4"], "Value4");
	EXPECT_EQ(settings.size(), 4);
}

//--------------------------------------------------------------------------

TEST(IniFormatTestSuite, load_empty_key)
{
	std::stringstream contents;
	contents << "Key1=Value1\n";
	contents << "=Value2\n";
	contents << "Key3=Value3\n";
	contents << "=Value4\n";

	IniFormat format;
	std::map<std::string, std::string> settings = format.load(contents);

	EXPECT_FALSE(settings.empty());
	EXPECT_EQ(settings.size(), 2);
	EXPECT_EQ(settings.count("Key1"), 1);
	EXPECT_EQ(settings.count("Key2"), 0);
	EXPECT_EQ(settings.count("Key3"), 1);
	EXPECT_EQ(settings.count("Key4"), 0);
}

//--------------------------------------------------------------------------

TEST(IniFormatTestSuite, load_empty_value)
{
	std::stringstream contents;
	contents << "Key1=\n";
	contents << "Key2=Value2\n";
	contents << "Key3=\n";
	contents << "Key4=Value4\n";

	IniFormat format;
	std::map<std::string, std::string> settings = format.load(contents);

	EXPECT_FALSE(settings.empty());
	EXPECT_EQ(settings.size(), 4);
	EXPECT_EQ(settings["Key1"], "");
	EXPECT_EQ(settings["Key2"], "Value2");
	EXPECT_EQ(settings["Key3"], "");
	EXPECT_EQ(settings["Key4"], "Value4");
	EXPECT_EQ(settings.size(), 4);
}

//--------------------------------------------------------------------------

TEST(IniFormatTestSuite, store)
{
	std::stringstream contents;
	IniFormat format;

	std::map<std::string, std::string> settings;
	settings["Key1"] = "Value1";
	settings["Key2"] = "Value2";
	settings["Key3"] = "Value3";
	settings["Key4"] = "Value4";
	format.store(settings, contents);

	EXPECT_FALSE(settings.empty());

	EXPECT_EQ(contents.str(), "Key1=Value1\nKey2=Value2\nKey3=Value3\nKey4=Value4\n");
}

//--------------------------------------------------------------------------

TEST(IniFormatTestSuite, read)
{
	std::stringstream contents;
	contents << "Key1=Value1\n";
	contents << "Key2=Value2\n";
	contents << "Key3=Value3\n";
	contents << "Key4=Value4\n";

	IniFormat format;
	EXPECT_EQ(format.read(contents), "Key1=Value1");
	EXPECT_EQ(format.read(contents), "Key2=Value2");
	EXPECT_EQ(format.read(contents), "Key3=Value3");
	EXPECT_EQ(format.read(contents), "Key4=Value4");
}

//--------------------------------------------------------------------------

TEST(IniFormatTestSuite, parse)
{
	std::stringstream contents;
	contents << "Key1=Value1\n";
	contents << "Key2=Value2\n";
	contents << "Key3=Value3\n";
	contents << "Key4=Value4\n";

	IniFormat format;

	auto item1 = std::make_pair<std::string, std::string>("Key1", "Value1");
	EXPECT_EQ(format.parse("Key1=Value1"), item1);

	auto item2 = std::make_pair<std::string, std::string>("Key2", "Value2");
	EXPECT_EQ(format.parse("Key2=Value2"), item2);

	auto item3 = std::make_pair<std::string, std::string>("Key3", "Value3");
	EXPECT_EQ(format.parse("Key3=Value3"), item3);

	auto item4 = std::make_pair<std::string, std::string>("Key4", "Value4");
	EXPECT_EQ(format.parse("Key4=Value4"), item4);
}

//--------------------------------------------------------------------------

} // test::unit
