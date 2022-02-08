#ifndef STORAGE_HPP_INCLUDED
#define STORAGE_HPP_INCLUDED

#include <string>
#include <string_view>


/**
 * General storage for settings
 */
class Storage {
public:
    explicit Storage() = default;
    virtual ~Storage() = default;

    virtual void load() = 0;
    virtual void store() = 0;

    virtual std::string get(
        const std::string_view key
    ) = 0;
    virtual void set(
        const std::string_view key,
        const std::string_view value
    ) = 0;
    virtual void del(
        const std::string_view key
    ) = 0;

};

#endif // STORAGE_HPP_INCLUDED
