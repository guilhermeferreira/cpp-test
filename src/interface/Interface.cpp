#include "Interface.hpp"

#include <cassert>      // for assert
#include <cstddef>      // for size_t
#include <stdexcept>    // for runtime_error
#include <type_traits>  // for integral_constant<>::value


//-----------------------------------------------------------------------------

std::tuple<std::string, std::string, std::string> Interface::parse(
    const std::string_view l)
{
    assert(!l.empty());

    std::string line{ l };
    assert(!line.empty());

    const std::string delimiter{" "};
    size_t pos{0};

    // Command must be always available
    std::string cmd{};
    pos = line.find(delimiter);
    if (pos != std::string::npos) {
        cmd = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
    }
    if (cmd.empty()) {
        throw std::runtime_error("Could not parse command");
    }

    // Key must be always available
    std::string key{};
    pos = line.find(delimiter);
    if (pos != std::string::npos) {
        key = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
    } else {
        key = line;
        line.clear();
    }
    if (key.empty()) {
        throw std::runtime_error("Could not parse key");
    }

    // NOTE: GET and DELETE do not have value. Thus, we don't throw neither
    //       assert when there is no value.
    std::string value = line;
    line.clear();

    assert(!cmd.empty());
    assert(!key.empty());
    auto parsed_line = std::make_tuple(cmd, key, value);
    assert(3 == std::tuple_size<decltype(parsed_line)>::value);

    return parsed_line;
}

//-----------------------------------------------------------------------------
