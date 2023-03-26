#include "InterfaceFactory.hpp"

#include <stdexcept>              // for runtime_error

#include "ScriptInterface.hpp"    // for ScriptInterface
#include "TerminalInterface.hpp"  // for TerminalInterface
#include "WebInterface.hpp"       // for WebInterface
#include "WindowInterface.hpp"    // for WindowInterface


//-----------------------------------------------------------------------------

const std::string InterfaceFactory::WINDOW_INTERFACE_ARG{ "gui" };
const std::string InterfaceFactory::WEB_INTERFACE_ARG{ "web" };

//-----------------------------------------------------------------------------

// NOTE: We return std::shared_ptr instead of std::unique_ptr in order to use
//       std::dynamic_pointer_cast for dynamic cast of smart pointers.
//       Without the std::dynamic_pointer_cast, the return type is a smart pointer
//       to Storage instead of the derived classes.
std::shared_ptr<Interface> InterfaceFactory::create(
    const std::string_view uiname,
    const int argc, 
    char *argv[])
{
    // NOTE: uiname might be empty for TerminalInterface
    // assert(!uiname.empty());

    if (uiname.empty()) {
        return std::make_shared<TerminalInterface>();
#ifdef WINDOW_INTERFACE
    } else if (InterfaceFactory::WINDOW_INTERFACE_ARG == uiname) {
        return std::make_shared<WindowInterface>(argc, argv);
#endif // WINDOW_INTERFACE
#ifdef WEB_INTERFACE
    } else if (InterfaceFactory::WEB_INTERFACE_ARG == uiname) {
        constexpr uint16_t PORT{ 8080 };
        return std::make_shared<WebInterface>(PORT);
#endif // WEB_INTERFACE
    } else {
        return std::make_shared<ScriptInterface>(std::string(uiname));
    }

    throw std::runtime_error("Unrecognized interface type");
}

//-----------------------------------------------------------------------------
