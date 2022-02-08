#include "IniFormat.hpp"

#include <cassert>  // for assert
#include <iostream>   // for iostream
#include <regex>    // for regex_match, match_results


//-----------------------------------------------------------------------------

std::map<std::string, std::string> IniFormat::load(std::iostream &contents)
{
    std::map<std::string, std::string> settings;

    while (!contents.eof()) {
        // Read line
        std::string line = read(contents);

        // Parse line
        std::pair<std::string, std::string> item = parse(line);
        std::string key = item.first;
        if (!key.empty()) {
            settings.emplace(item);
        }
    }
    contents.clear();

    return settings;
}

//-----------------------------------------------------------------------------

void IniFormat::store(
    const std::map<std::string, std::string> &settings,
    std::iostream &contents)
{
    assert(!settings.empty());

    for (auto item : settings) {
        std::string key = item.first;
        std::string value = item.second;
        if (!key.empty()) {
            contents << key << "=" << value << std::endl;
        }
    }
}

//-----------------------------------------------------------------------------

std::string IniFormat::read(std::iostream &contents)
{
    std::string line{};
    std::getline(contents, line);

    return line;
}

//-----------------------------------------------------------------------------

std::pair<std::string, std::string> IniFormat::parse(
    const std::string &line)
{
    std::pair<std::string, std::string> item;

    std::smatch m{};
    std::regex re{ "(.*)=(.*)" };
    bool found = std::regex_match(line, m, re);
    if (found) {
        std::string key = m[1];
        std::string value = m[2];
        item = std::make_pair(key, value);
    }

    return item;
}

//-----------------------------------------------------------------------------
