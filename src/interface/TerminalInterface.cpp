#include "TerminalInterface.hpp"

#include <iostream>     // for char_traits, operator<<, cout, endl, ostream

#include "strings.hpp"  // for PROMPT


//-----------------------------------------------------------------------------

std::string TerminalInterface::request()
{
    std::cout << PROMPT;
    std::string line{};
    std::getline(std::cin, line);

    return line;
}

//-----------------------------------------------------------------------------

void TerminalInterface::reply(const std::string_view resp)
{
    std::cout << resp << std::endl;
}

//-----------------------------------------------------------------------------
