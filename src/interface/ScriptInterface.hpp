#ifndef SCRIPT_INTERFACE_HPP_INCLUDED
#define SCRIPT_INTERFACE_HPP_INCLUDED

#include <fstream>        // for ifstream
#include <string>         // for string
#include <string_view>    // for string_view

#include "Interface.hpp"  // for Interface


/**
 * Command from a script file.
 * 
 * Read commands from a file to perform scripted executions for fuzzing.
 */
class ScriptInterface : public Interface {
public:
    explicit ScriptInterface(const std::string &scriptname);

    std::string request() override;
    void reply(const std::string_view resp) override;

private:
    std::ifstream _script;

};

#endif // SCRIPT_INTERFACE_HPP_INCLUDED
