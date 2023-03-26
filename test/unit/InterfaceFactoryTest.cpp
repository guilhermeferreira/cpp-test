#include "interface/InterfaceFactory.hpp"

#include <stdexcept>
#include <typeinfo>

#include <gtest/gtest.h>

#include "interface/TerminalInterface.hpp"
#include "interface/ScriptInterface.hpp"
#include "interface/WindowInterface.hpp"


namespace test::unit {

//--------------------------------------------------------------------------

TEST(InterfaceFactoryTestSuite, create_terminal_interface)
{
	// Empty ui means terminal
	const std::string ui{};

	// Check base class
	std::shared_ptr<Interface> interface{ InterfaceFactory::create(ui, 0, nullptr) };
	EXPECT_TRUE(interface);

	// Check correct derived class
	std::shared_ptr<TerminalInterface> terminal{ std::dynamic_pointer_cast<TerminalInterface>(interface) };
	EXPECT_TRUE(terminal);

	// Check wrong derived class
	std::shared_ptr<ScriptInterface> script{ std::dynamic_pointer_cast<ScriptInterface>(interface) };
	EXPECT_FALSE(script);
}

//--------------------------------------------------------------------------

TEST(InterfaceFactoryTestSuite, create_script_interface)
{
	// Empty ui means terminal
	const std::string ui{ TEST_DST_DIR "script.txt" };

	// Check base class
	std::shared_ptr<Interface> interface{ InterfaceFactory::create(ui, 0, nullptr) };
	EXPECT_TRUE(interface);

	// Check wrong derived class
	std::shared_ptr<TerminalInterface> terminal{ std::dynamic_pointer_cast<TerminalInterface>(interface) };
	EXPECT_FALSE(terminal);

	// Check correct derived class
	std::shared_ptr<ScriptInterface> script{ std::dynamic_pointer_cast<ScriptInterface>(interface) };
	EXPECT_TRUE(script);
}

//--------------------------------------------------------------------------

#ifdef WINDOW_INTERFACE
TEST(InterfaceFactoryTestSuite, create_window_interface)
{
	// Empty ui means terminal
	const std::string ui{ InterfaceFactory::WINDOW_INTERFACE_ARG };
	constexpr int argc { 2 };
	char argv0[] { "prog" };
	char argv1[] { "arg1" };
	char *argv[argc] { argv0, argv1 };

	// Check base class
	std::shared_ptr<Interface> interface{ InterfaceFactory::create(ui, argc, argv) };
	EXPECT_TRUE(interface);

	// Check wrong derived class
	std::shared_ptr<TerminalInterface> terminal{ std::dynamic_pointer_cast<TerminalInterface>(interface) };
	EXPECT_FALSE(terminal);

	// Check wrong derived class
	std::shared_ptr<ScriptInterface> script{ std::dynamic_pointer_cast<ScriptInterface>(interface) };
	EXPECT_FALSE(script);

	// Check correct derived class
	std::shared_ptr<WindowInterface> window{ std::dynamic_pointer_cast<WindowInterface>(interface) };
	EXPECT_TRUE(window);
}
#endif // WINDOW_INTERFACE

//--------------------------------------------------------------------------

} // test::unit
