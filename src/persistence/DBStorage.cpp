#include "DBStorage.hpp"

#include <cassert>

#include <stdexcept>
#include <vector>

#include <fmt/core.h>


//-----------------------------------------------------------------------------

static std::vector<std::string> execResp;

//-----------------------------------------------------------------------------

DBStorage::DBStorage(const std::string_view filename)
: _db{ nullptr }
{
    assert(_db == nullptr);
    assert(!filename.empty());

    int rc = sqlite3_open(filename.data(), &_db);
    if (rc != SQLITE_OK) {
        sqlite3_close(_db);

        std::string msg{"Can't open database: "};
        msg += sqlite3_errmsg(_db);
        throw std::runtime_error(msg);
    }

    // Check if table exists, otherwise create it
    // ./sqlite data.db "SELECT name FROM sqlite_master WHERE type='table' AND name='storage'"
    exec("SELECT name FROM sqlite_master WHERE type='table' AND name='storage'");

    if (execResp.empty()) {
        // ./sqlite data.db "CREATE TABLE storage ( key varchar(255), value varchar(255) )"
        exec("CREATE TABLE storage ( key varchar(255), value varchar(255) )");
    }

    assert(_db);
}

//-----------------------------------------------------------------------------

DBStorage::~DBStorage()
{
    assert(_db);

    sqlite3_close(_db);
}

//-----------------------------------------------------------------------------

void DBStorage::load()
{
    assert(_db);
}

//-----------------------------------------------------------------------------

void DBStorage::store()
{
    assert(_db);
}

//-----------------------------------------------------------------------------

std::string DBStorage::get(
    const std::string_view key)
{
    assert(!key.empty());
    assert(_db);

    // ./sqlite data.db "SELECT value FROM storage WHERE key='key'"
    exec(fmt::format("SELECT value FROM storage WHERE key='{}'", key));

    // NOTE: throw an exception to distinguish when a key has an empty value
    //       from when the key was never added. If we return empty string to
    //       to mean "the key does not exist", we can't distinguish the two cases.
    if (execResp.empty()) {
        throw std::runtime_error("Key not found");
    }

    std::string value{ execResp[0] };

    return value;
}

//-----------------------------------------------------------------------------

void DBStorage::set(
    const std::string_view key,
    const std::string_view value)
{
    assert(!key.empty());
    // NOTE: allow set of empty values (key with no value)
    // assert(!value.empty());
    assert(_db);

    try {
        if (get(key) != value) {
            update(key, value);
        }
    }
    // NOTE: If the get() throws an exception, it means the key doesn't exist
    //       and we should use INSERT instead of UPDATE.
    catch (const std::runtime_error &) {
        insert(key, value);
    }
}

//-----------------------------------------------------------------------------

void DBStorage::del(
    const std::string_view key)
{
    assert(!key.empty());
    assert(_db);

    // ./sqlite data.db "DELETE FROM storage WHERE key='key'"
    exec(fmt::format("DELETE FROM storage WHERE key='{}'", key));
}

//-----------------------------------------------------------------------------

void DBStorage::update(
    const std::string_view key,
    const std::string_view value)
{
    assert(!key.empty());
    // NOTE: allow set of empty values (key with no value)
    // assert(!value.empty());
    assert(_db);

    // ./sqlite data.db "UPDATE storage SET value='value' WHERE key='key'"
    exec(fmt::format("UPDATE storage SET value='{}' WHERE key='{}'", value, key));
}

//-----------------------------------------------------------------------------

void DBStorage::insert(
    const std::string_view key,
    const std::string_view value)
{
    assert(!key.empty());
    // NOTE: allow set of empty values (key with no value)
    // assert(!value.empty());
    assert(_db);

    // ./sqlite data.db "INSERT INTO storage (key, value) VALUES ('key', 'value')"
    exec(fmt::format("INSERT INTO storage (key, value) VALUES ('{}','{}')", key, value));
}

//-----------------------------------------------------------------------------

static int execCallback(
    void */*NotUsed*/,
    int entryCount,
    char **values,
    char **/*columns*/)
{
    assert(execResp.empty());

    for (int entry = 0; entry < entryCount; entry++) {
        std::string value{ values[entry] ? values[entry] : "NULL" };
        execResp.push_back(value);
    }

    return 0;
}

//-----------------------------------------------------------------------------

void DBStorage::exec(
    const std::string &query)
{
    assert(!query.empty());
    assert(_db);

    execResp.clear();

    char *errMsg = nullptr;
    int rc = sqlite3_exec(_db, query.c_str(), execCallback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);

        std::string msg{"SQL error: "};
        msg += errMsg;

        throw std::runtime_error(msg);
    }
}

//-----------------------------------------------------------------------------
