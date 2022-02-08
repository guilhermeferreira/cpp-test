#include "FileStorage.hpp"

#include <cassert>                    // for assert
#include <stdexcept>                  // for runtime_error

#include "persistence/Format.hpp"     // for Format
#include "persistence/IniFormat.hpp"  // for IniFormat

//-----------------------------------------------------------------------------

FileStorage::FileStorage(const std::string_view filename)
: _settings{},
  _filename{ std::string(filename) },
  _file{},
  _format{ std::make_unique<IniFormat>() }
{
    assert(_settings.size() == 0);
    assert(!_filename.empty());
    assert(_format);
    assert(!_file.is_open());

    if (!std::filesystem::exists(_filename)) {
        // NOTE: Open the file for writing and close it. This nasty trick is
        //       required to create the file if it doesn't exist (it works like
        //       shell's touch command)
        _file.open(_filename.string(), std::ios::out);
        _file.close();
    }

    if (!std::filesystem::is_regular_file(_filename)) {
        throw std::runtime_error("File is not a regular file");
    }

    _file.open(_filename.string(), std::ios::in | std::ios::out);
    if (!_file.is_open()) {
        throw std::runtime_error("Could not open the file");
    }
}

//-----------------------------------------------------------------------------

FileStorage::~FileStorage()
{
    assert(_file.is_open());

    _file.close();
}

//-----------------------------------------------------------------------------

void FileStorage::load()
{
    assert(_format);
    assert(_file.is_open());

    _file.seekg(0);

    _settings = _format->load(_file);
}

//-----------------------------------------------------------------------------

void FileStorage::store()
{
    assert(_format);
    assert(_file.is_open());
    assert(!_filename.empty());

    // NOTE: fix the problem Kalin pointed out in the interview. What happen if
    //       there's a power outage between the file truncation and the complete
    //       write?

    //       1) Close the original file (the file from where we read the settings)
    _file.close();

    //       2) Create a temporary file
    const std::string tmpfilename{ _filename.string() + ".tmp" };
    if (std::filesystem::exists(tmpfilename)) {
        throw std::runtime_error("Could not create temporary file");
    }
    _file.open(tmpfilename, std::ios::out | std::ios::trunc);

    //       3) Write the setting to the temporary file
    if (!_settings.empty()) {
        _format->store(_settings, _file);

        _file.flush();
    }

    //       4) Replace the original file by the temporary.
    std::filesystem::rename(tmpfilename, _filename);
}

//-----------------------------------------------------------------------------

std::string FileStorage::get(
    const std::string_view key)
{
    assert(!key.empty());

    // NOTE: throw an exception to distinguish when a key has an empty value
    //       from when the key was never added. If we return empty string to
    //       to mean "the key does not exist", we can't distinguish the two cases.
    if (_settings.count(std::string(key)) < 1) {
        throw std::runtime_error("Key not found");
    }

    return _settings.at(std::string(key));
}

//-----------------------------------------------------------------------------

void FileStorage::set(
    const std::string_view key,
    const std::string_view value)
{
    assert(!key.empty());
    // NOTE: allow set of empty values (key with no value)
    // assert(!value.empty());

    _settings[std::string(key)] = std::string(value);
}

//-----------------------------------------------------------------------------

void FileStorage::del(
    const std::string_view key)
{
    assert(!key.empty());

    _settings.erase(std::string(key));
}

//-----------------------------------------------------------------------------
