#ifndef WINDOW_INTERFACE_HPP_INCLUDED
#define WINDOW_INTERFACE_HPP_INCLUDED

#include <future>         // for promise
#include <memory>         // for shared_ptr
#include <string>         // for string
#include <string_view>    // for string_view
#include <thread>         // for thread

#include "Interface.hpp"  // for Interface

class Window;

/**
 * Graphical User Interface
 */
class WindowInterface : public Interface
{
public:
    WindowInterface(int argc, char *argv[]);
    virtual ~WindowInterface();

    std::string request() override;
    void reply(const std::string_view resp) override;

private:
    std::promise<std::shared_ptr<Window>> _promise;
    std::shared_ptr<Window> _window;
    std::thread _thread;

};

#endif // WINDOW_INTERFACE_HPP_INCLUDED
