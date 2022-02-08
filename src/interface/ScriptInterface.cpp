#include "ScriptInterface.hpp"

#include <cassert>     // for assert
#include <filesystem>  // for exists, is_regular_file
#include <iostream>    // for cout
#include <stdexcept>   // for runtime_error


//-----------------------------------------------------------------------------

ScriptInterface::ScriptInterface(const std::string &scriptname)
: _script{}
{
    assert(!scriptname.empty());

    if (!std::filesystem::exists(scriptname)) {
        throw std::runtime_error("File does not exist");
    }

    if (!std::filesystem::is_regular_file(scriptname)) {
        throw std::runtime_error("File is not a regular file");
    }

    _script.open(scriptname, std::ios::in);
    if (!_script.is_open()) {
        throw std::runtime_error("Could not open the script file");
    }

    assert(_script.is_open());
}

//-----------------------------------------------------------------------------

std::string ScriptInterface::request()
{
    assert(_script.is_open());

    std::string line{};
    std::getline(_script, line);

    return line;
}

//-----------------------------------------------------------------------------

void ScriptInterface::reply(const std::string_view resp)
{
    assert(_script.is_open());

    std::cout << resp << std::endl;
}

//-----------------------------------------------------------------------------
