#ifndef WEB_INTERFACE_HPP_INCLUDED
#define WEB_INTERFACE_HPP_INCLUDED

#include <condition_variable>   // for condition_variable
#include <list>                 // for list
#include <mutex>                // for mutex
#include <string>               // for string
#include <string_view>          // for string_view
#include <thread>               // for thread

#include "Interface.hpp"        // for Interface


/**
 * Interface to be seen in browsers
 */
class WebInterface : public Interface {
public:
    explicit WebInterface(const uint16_t port);
    virtual ~WebInterface() = default;

    void sendCommand(const std::string &cmd);
    std::string recvReply();

    std::string request() override;
    void reply(const std::string_view resp) override;

private:
    std::list<std::string> _commands;
    std::mutex _commands_lock;
    std::condition_variable _commands_cond;

    std::string _reply;
    std::mutex _reply_lock;
    std::condition_variable _reply_cond;

    std::thread _thread;

};

#endif // WEB_INTERFACE_HPP_INCLUDED
