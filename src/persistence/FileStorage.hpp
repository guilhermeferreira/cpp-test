#ifndef FILE_STORAGE_HPP_INCLUDED
#define FILE_STORAGE_HPP_INCLUDED

#include <filesystem>   // for path
#include <fstream>      // for fstream
#include <map>          // for map
#include <memory>       // for unique_ptr
#include <string>       // for string
#include <string_view>  // for string_view

#include "Storage.hpp"  // for Storage

class Format;           // for Format


/**
 * Store settings into a local file
 */
class FileStorage : public Storage {
public:
    explicit FileStorage(const std::string_view filename);
    virtual ~FileStorage();

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
    std::map<std::string, std::string> _settings;
    std::filesystem::path _filename;
    std::fstream _file;
    std::unique_ptr<Format> _format;

};

#endif // FILE_STORAGE_HPP_INCLUDED
