#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <string>
#include <string_view>


/**
 * Graphical User Window
 */
class Window {
public:
    Window() = default;
    virtual ~Window() = default;

    virtual std::string request() = 0;
    virtual void reply(const std::string_view resp) = 0;

    virtual void show() = 0;

};

#endif // WINDOW_HPP_INCLUDED