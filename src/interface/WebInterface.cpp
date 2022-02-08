#include "WebInterface.hpp"

#include <cstddef>      // for size_t
#include <cassert>      // for assert
#include <filesystem>   // for path
#include <fstream>      // for ifstream, basic_ostream::operator<<, ostrings...
#include <map>          // for map, map<>::mapped_type
#include <vector>       // for vector

#include "crow_all.h"   // for load, response, SimpleApp, CROW_ROUTE, Tagged...

#include "strings.hpp"  // for DELETE_CMD, EXIT_CMD, GET_CMD, SET_CMD, STORE...


//-----------------------------------------------------------------------------

static const std::string WEB_DIR{ "WebInterface/" };
static std::filesystem::path web_path{ __FILE__ };

//-----------------------------------------------------------------------------

void sendFile(
    const std::string &filename,
    const std::string &contentType,
    crow::response &res);

void sendStyle(
    const std::string &filename,
    crow::response &res);

void sendFont(
    const std::string &filename,
    crow::response &res);

void sendHtml(
    const std::string &filename,
    crow::response &res);

void sendHtml(
    const std::string &filename,
    crow::response &res,
    crow::mustache::context &ctx);

void WebThread(
    const uint16_t port,
    WebInterface * const interface);

//-----------------------------------------------------------------------------

void sendFile(
    const std::string &filename,
    const std::string &contentType,
    crow::response &res)
{
    std::ifstream in(web_path.string() + filename, std::ifstream::in);
    if (in) {
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();
        res.set_header("Content-Type", contentType);
        res.write(contents.str());
    } else {
        res.code = 404;
        res.write("Not found");
    }
    res.end();
}

//-----------------------------------------------------------------------------

void sendStyle(
    const std::string &filename,
    crow::response &res)
{
    sendFile("styles/" + filename, "text/css", res);
}

//-----------------------------------------------------------------------------

void sendFont(
    const std::string &filename,
    crow::response &res)
{
    sendFile("fonts/" + filename, "font/ttf", res);
}

//-----------------------------------------------------------------------------

void sendHtml(
    const std::string &filename,
    crow::response &res)
{
    sendFile(filename + ".html", "text/html", res);
}

//-----------------------------------------------------------------------------

void sendHtml(
    const std::string &filename,
    crow::response &res,
    crow::mustache::context &ctx)
{
    std::string contents{
        crow::mustache::load(web_path.string() + filename + ".html").render(ctx)
    };

    if (!contents.empty()) {
        res.set_header("Content-Type", "text/html");
        res.write(contents);
        res.code = 200;
        res.end();
    }
}

//-----------------------------------------------------------------------------

void WebThread(
    const uint16_t port,
    WebInterface * const interface)
{
    assert(interface);

    crow::SimpleApp app;
    crow::mustache::set_base(".");
    
    web_path = web_path.replace_filename(WEB_DIR);

    std::map<std::string, std::vector<std::string>> commands;
    commands["get"]   = { GET_CMD,    "key" };
    commands["set"]   = { SET_CMD,    "key", "value" };
    commands["del"]   = { DELETE_CMD, "key" };
    commands["store"] = { STORE_CMD };
    commands["exit"]  = { EXIT_CMD };

    CROW_ROUTE(app, "/<string>")
        ([interface, commands] (const crow::request &req, crow::response &res, const std::string &page) {
            if (crow::HTTPMethod::Get == req.method) {
                if (commands.count(page)) {
                    std::vector<std::string> args{ commands.at(page) };
                    size_t i = 0;
                    std::string cmd{ args[i] };
                    for (; i < args.size(); i++) {
                        const char *arg{ req.url_params.get(args[i]) };
                        if (arg != nullptr) {
                            cmd += " ";
                            cmd += arg;
                        }
                    }

                    interface->sendCommand(cmd);

                    std::string value{ interface->recvReply() };
                    crow::json::wvalue dto;
                    dto["response"] = crow::json::load("{\"value\": \"" + value + "\"}");

                    sendHtml(page, res, dto);
                }
                else {
                    sendHtml(page, res);
                }
            }
        });

    CROW_ROUTE(app, "/styles/<string>")
        ([] (const crow::request &/*req*/, crow::response &res, const std::string &filename) {
            sendStyle(filename, res);
        });

    CROW_ROUTE(app, "/fonts/<string>")
        ([] (const crow::request &/*req*/, crow::response &res, const std::string &filename) {
            sendFont(filename, res);
        });

    CROW_ROUTE(app, "/")
        ([] (const crow::request &/*req*/, crow::response &res) {
            sendHtml("index", res);
        });

    app.port(port).multithreaded().run();
}

//-----------------------------------------------------------------------------

WebInterface::WebInterface(const uint16_t port)
: _commands{}
, _commands_lock{}
, _commands_cond{}
, _reply{}
, _reply_lock{}
, _reply_cond{}
, _thread( WebThread, port, this )
{
    _thread.detach();
}

//-----------------------------------------------------------------------------

void WebInterface::sendCommand(const std::string &cmd)
{
    assert(!cmd.empty());

    std::lock_guard<std::mutex> lk{ _commands_lock };

    _commands.push_back(cmd);

    _commands_cond.notify_all();
}

//-----------------------------------------------------------------------------

std::string WebInterface::recvReply()
{
    std::unique_lock<std::mutex> lk{ _reply_lock };

    _reply_cond.wait(lk, [this]()
        {
            return !_reply.empty();
        });

    assert(!_reply.empty());
    std::string tmp{ _reply };
    _reply.clear();
    assert(_reply.empty());

    return tmp;
}

//-----------------------------------------------------------------------------

std::string WebInterface::request()
{
    std::unique_lock<std::mutex> lk{ _commands_lock };

    _commands_cond.wait(lk, [this]()
        {
            return !_commands.empty();
        });

    std::string cmd{ _commands.front() };
    _commands.pop_front();

    return cmd;
}

//-----------------------------------------------------------------------------

void WebInterface::reply(const std::string_view res)
{
    assert(!res.empty());

    _reply = res;

    _reply_cond.notify_one();
}

//-----------------------------------------------------------------------------
