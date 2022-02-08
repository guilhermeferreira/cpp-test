#ifndef INIT_FORMAT_HPP_INCLUDED
#define INIT_FORMAT_HPP_INCLUDED

#include <iostream>    // for iostream
#include <map>         // for map
#include <string>      // for string
#include <utility>     // for pair

#include "Format.hpp"  // for Format


/**
 * Format settings with INI style
 */
class IniFormat : public Format {
public:
    IniFormat() = default;
    virtual ~IniFormat() = default;

    std::map<std::string, std::string> load(std::iostream &contents) override;
    void store(
        const std::map<std::string, std::string> &settings,
        std::iostream &contents) override;

    std::string read(std::iostream &contents);
    std::pair<std::string, std::string> parse(const std::string &line);

};

#endif // INIT_FORMAT_HPP_INCLUDED
