#ifndef FORMAT_HPP_INCLUDED
#define FORMAT_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>


/**
 * General format of the settings
 */
class Format {
public:
    Format() = default;
    virtual ~Format() = default;

    virtual std::map<std::string, std::string> load(std::iostream &contents) = 0;
    virtual void store(
        const std::map<std::string, std::string> &settings,
        std::iostream &contents) = 0;

};

#endif // FORMAT_HPP_INCLUDED