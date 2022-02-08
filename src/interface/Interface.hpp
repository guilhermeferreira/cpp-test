#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <string>
#include <string_view>
#include <tuple>


/**
 * General command from user
 */
class Interface {
public:
    Interface() = default;
    virtual ~Interface() = default;

    static std::tuple<std::string, std::string, std::string> parse(const std::string_view line);

    virtual std::string request() = 0;
    virtual void reply(const std::string_view resp) = 0;

};

#endif // INTERFACE_HPP_INCLUDED