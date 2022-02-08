#ifndef STORAGE_FACTORY_HPP_INCLUDED
#define STORAGE_FACTORY_HPP_INCLUDED

#include <memory>
#include <string_view>

class Storage;


/**
 * Create Storage objects
 */
class StorageFactory  {
public:
    static std::shared_ptr<Storage> create(const std::string_view filename);

    StorageFactory() = delete;
    ~StorageFactory() = delete;

};

#endif // STORAGE_FACTORY_HPP_INCLUDED
