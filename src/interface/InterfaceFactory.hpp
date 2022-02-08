#ifndef INTERFACE_FACTORY_HPP_INCLUDED
#define INTERFACE_FACTORY_HPP_INCLUDED

#include <memory>
#include <string>
#include <string_view>

class Interface;


/**
 * Create Interface objects
 */
class InterfaceFactory  {
public:
    static const std::string WINDOW_INTERFACE_ARG;
    static const std::string WEB_INTERFACE_ARG;

    static std::shared_ptr<Interface> create(
        const std::string_view uiname,
        const int argc,
        char *argv[]
    );

    InterfaceFactory() = delete;
    ~InterfaceFactory() = delete;

};

#endif // INTERFACE_FACTORY_HPP_INCLUDED
