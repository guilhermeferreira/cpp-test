#include "Core.hpp"

#include <cassert>

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "interface/Interface.hpp"
#include "strings.hpp"


//-----------------------------------------------------------------------------

Core::Core(
    std::shared_ptr<Interface> interface,
    std::shared_ptr<Storage> storage
) : _interface{ interface },
    _storage{ storage },
    _commands{},
    _special_commands{},
    _executing{ false }
{
    assert(_interface);
    assert(_storage);
    assert(!_executing);

    setup();
}

//-----------------------------------------------------------------------------

void Core::setup()
{
    setup_commands();
    setup_special_commands();
}

//-----------------------------------------------------------------------------

void Core::loop()
{
    assert(_interface);
    assert(_storage);
    assert(!_executing);

    _storage->load();

    _executing = true;

    std::string line{};
    do {
        line = _interface->request();

        if (!line.empty()) {
            std::string resp{};
            if (is_special_command(line)) {
                resp = special_command(line);
            } else {
                resp = command(line);
            }

            _interface->reply(resp);
        }

    } while (_executing);

    _storage->store();
}

//-----------------------------------------------------------------------------

std::string Core::command(const std::string_view input)
{
    assert(_storage);
    assert(!input.empty());
    assert(!_commands.empty());

    std::string resp = FAILURE_RESP;

    try {
        std::string cmd{};
        std::string key{};
        std::string value{};

        std::tie(cmd, key, value) = Interface::parse(input);

        if (_commands.count(cmd) == 1) {
            resp = _commands.at(cmd)(key, value);
        }
    } catch (const std::runtime_error &ex) {
    }

    return resp;
}

//-----------------------------------------------------------------------------

void Core::setup_commands()
{
    assert(_commands.empty());

    _commands[SET_CMD] = Command(
        [&] (const std::string_view key, const std::string_view value) -> std::string {
            if (!key.empty() && !value.empty()) {
                _storage->set(key, value);
                return SUCCESS_RESP;
            } else {
                return FAILURE_RESP;
            }
        });

    _commands[GET_CMD] = Command(
        [&] (const std::string_view key, const std::string_view) -> std::string {
            if (!key.empty()) {
                try {
                    return _storage->get(key);
                } catch (const std::runtime_error &) {
                    return FAILURE_RESP;
                }
            } else {
                return FAILURE_RESP;
            }
        });

    _commands[DELETE_CMD] = Command(
        [&] (const std::string_view key, const std::string_view) -> std::string {
            if (!key.empty()) {
                _storage->del(key);
                return SUCCESS_RESP;
            } else {
                return FAILURE_RESP;
            }
        });

    assert(!_commands.empty());
}

//-----------------------------------------------------------------------------

bool Core::is_special_command(const std::string_view input) const
{
    assert(!input.empty());

    return (_special_commands.count(input) == 1);
}

//-----------------------------------------------------------------------------

std::string Core::special_command(const std::string_view input)
{
    assert(_storage);
    assert(!input.empty());
    assert(!_special_commands.empty());
    assert(_special_commands.count(input) == 1);

    if (_special_commands.count(input) == 1) {
        return _special_commands.at(input)();
    }

    return FAILURE_RESP;
}

//-----------------------------------------------------------------------------

void Core::setup_special_commands()
{
    assert(_special_commands.empty());
    assert(!_executing);

    _special_commands[EXIT_CMD] = SpecialCommand(
        [&] () -> std::string {
            _storage->store();
            _executing = false;
            return SUCCESS_RESP;
        });

    _special_commands[STORE_CMD] = SpecialCommand(
        [&] () -> std::string {
            _storage->store();
            return SUCCESS_RESP;
        });

    assert(!_special_commands.empty());
    assert(!_executing);
}

//-----------------------------------------------------------------------------
