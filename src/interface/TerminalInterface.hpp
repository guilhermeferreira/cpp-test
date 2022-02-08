#ifndef TERMINAL_INTERFACE_HPP_INCLUDED
#define TERMINAL_INTERFACE_HPP_INCLUDED

#include <string>         // for string
#include <string_view>    // for string_view

#include "Interface.hpp"  // for Interface


/**
 * Command from terminal interaction (stdin/stdout)
 */
class TerminalInterface : public Interface {
public:
    std::string request() override;
    void reply(const std::string_view resp) override;

};

#endif // TERMINAL_INTERFACE_HPP_INCLUDED
