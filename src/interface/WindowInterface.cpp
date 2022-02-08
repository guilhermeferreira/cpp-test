#include "WindowInterface.hpp"

#include <cassert>               // for assert
#include <functional>            // for ref

#include <QApplication>         // for QApplication

#include "QtWindow.hpp"          // for QtWindow
#include "interface/Window.hpp"  // for Window


//-----------------------------------------------------------------------------

void WindowThread(
    std::promise<std::shared_ptr<Window>> &prom,
    int argc,
    char *argv[]);
void WindowThread(
    std::promise<std::shared_ptr<Window>> &prom,
    int argc,
    char *argv[])
{
    assert(argc);
    assert(argv);

    QApplication app{ argc, argv };

    // NOTE: Qt requires Window (QMainWindow) object creation after QApplication.
    //       Thus, we create the QMainWindow in the same thread as QApplication,
    //       and sent the create QMainWindow to the main thread via std::promise.
    std::shared_ptr<Window> window{new QtWindow};
    window->show();

    // Fulfill the promise
    prom.set_value(window);

    app.exec();
}

//-----------------------------------------------------------------------------

WindowInterface::WindowInterface(int argc, char *argv[])
: _promise{}
, _window{}
, _thread{ WindowThread, std::ref(_promise), argc, argv }
{
    assert(argc);
    assert(argv);

    assert(!_window);
    // NOTE: Use std::promise/std::future for one-time event. Wait for the
    //       WindowThread create the Window and share a pointer to it.
    std::future<std::shared_ptr<Window>> fut = _promise.get_future();
    // Block until the promise is fulfilled
    _window = fut.get();
    assert(_window);
}

//-----------------------------------------------------------------------------

WindowInterface::~WindowInterface()
{
    assert(_window);
    assert(_thread.joinable());

    // NOTE: Join the thread to wait for it to finish and avoid it to call
    //       terminate() and throw a SIGABRT
    _thread.join();
}

//-----------------------------------------------------------------------------

std::string WindowInterface::request()
{
    assert(_window);

    return _window->request();
}

//-----------------------------------------------------------------------------

void WindowInterface::reply(const std::string_view resp)
{
    assert(_window);

    _window->reply(resp);
}

//-----------------------------------------------------------------------------
