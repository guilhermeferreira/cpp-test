#ifndef DB_STORAGE_HPP_INCLUDED
#define DB_STORAGE_HPP_INCLUDED

#include <string>
#include <string_view>

#include <sqlite3.h>

#include "Storage.hpp"


/**
 * Store settings into a database
 */
class DBStorage : public Storage {
public:
    explicit DBStorage(const std::string_view filename);
    DBStorage(const DBStorage&) = delete;
    virtual ~DBStorage();

    DBStorage& operator=(const DBStorage&) = delete;

    void load() override;
    void store() override;

    std::string get(
        const std::string_view key
    ) override;
    void set(
        const std::string_view key,
        const std::string_view value
    ) override;
    void del(
        const std::string_view key
    ) override;

private:
    void update(
        const std::string_view key,
        const std::string_view value
    );
    void insert(
        const std::string_view key,
        const std::string_view value
    );
    void exec(const std::string &query);

    sqlite3 *_db;

};

#endif // DB_STORAGE_HPP_INCLUDED