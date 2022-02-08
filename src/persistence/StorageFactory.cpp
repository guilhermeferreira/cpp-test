#include "StorageFactory.hpp"

#include <cassert>

#include <filesystem>
#include <stdexcept>        // for runtime_error
#include <string>           // for operator==, string

#include "DBStorage.hpp"    // for DBStorage
#include "FileStorage.hpp"  // for FileStorage


//-----------------------------------------------------------------------------

// NOTE: We return std::shared_ptr instead of std::unique_ptr in  order to use
//       std::dynamic_pointer_cast for dynamic cast of smart pointers.
//       Without the std::dynamic_pointer_cast, the return type is a smart pointer
//       to Storage instead of the derived classes.
std::shared_ptr<Storage> StorageFactory::create(const std::string_view filename)
{
    assert(!filename.empty());

    static const std::string FILE_STORAGE_EXT{ ".ini" };
    static const std::string DB_STORAGE_EXT{ ".db" };

    const std::string ext{ std::filesystem::path(filename).extension() };

    if (FILE_STORAGE_EXT == ext) {
        return std::make_shared<FileStorage>(filename);
    }
    else if (DB_STORAGE_EXT == ext) {
        return std::make_shared<DBStorage>(filename);
    }

    throw std::runtime_error("Unrecognized file type");
}

//-----------------------------------------------------------------------------
