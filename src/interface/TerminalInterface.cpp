#include "TerminalInterface.hpp"

#include <string>     // for getline, string

#include <fmt/core.h>

#include "strings.hpp"  // for PROMPT


//-----------------------------------------------------------------------------

std::string TerminalInterface::request()
{
    fmt::print("{}", PROMPT);
    std::string line{};
    std::getline(std::cin, line);

    return line;
}

//-----------------------------------------------------------------------------

void TerminalInterface::reply(const std::string_view resp)
{
    fmt::println("{}", resp);
}

//-----------------------------------------------------------------------------
