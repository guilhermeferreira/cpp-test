#ifndef CORE_HPP_INCLUDED
#define CORE_HPP_INCLUDED

#include <functional>
#include <memory>
#include <ostream>
#include <string_view>
#include <unordered_map>

#include "interface/Interface.hpp"
#include "persistence/Storage.hpp"


/**
 * Command
 */
using Command = std::function<std::string(const std::string_view, const std::string_view)>;
using SpecialCommand = std::function<std::string()>;


/**
 * Business logic
 */
class Core {
public:
    explicit Core(
        std::shared_ptr<Interface> interface,
        std::shared_ptr<Storage> storage
    );
    ~Core() = default;

    void setup();
    void loop();

    std::string command(const std::string_view input);
    void setup_commands();

    bool is_special_command(const std::string_view input) const;
    std::string special_command(const std::string_view input);
    void setup_special_commands();

private:
    std::shared_ptr<Interface> _interface;
    std::shared_ptr<Storage> _storage;
    std::unordered_map<std::string_view, Command> _commands;
    std::unordered_map<std::string_view, SpecialCommand> _special_commands;
    bool _executing;

};

#endif // CORE_HPP_INCLUDED