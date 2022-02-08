#include "QtWindow.hpp"
#include "ui_QtWindow.h"

#include <cassert>

#include <QString>

#include "strings.hpp"


//-----------------------------------------------------------------------------

QtWindow::QtWindow(QWidget *parent)
: QMainWindow(parent)
, Window{}
, _ui(new Ui::QtWindow)
, _commands{}
, _commands_lock{}
, _commands_cond{}
{
    _ui->setupUi(this);

    QObject::connect(_ui->getButton, &QPushButton::clicked,
                     this, &QtWindow::getClicked);
    QObject::connect(_ui->setButton, &QPushButton::clicked,
                     this, &QtWindow::setClicked);
    QObject::connect(_ui->delButton, &QPushButton::clicked,
                     this, &QtWindow::delClicked);
}

//-----------------------------------------------------------------------------

QtWindow::~QtWindow()
{
    delete _ui;
}

//-----------------------------------------------------------------------------

std::string QtWindow::request()
{
    std::unique_lock<std::mutex> lk{_commands_lock};

    // NOTE: use std::conditional_variable for multiple events.
    // Block main thread (calling QtWindow::request()) until one command is
    // queued by the UI thread (calling QtWindow::sendCommand())
    _commands_cond.wait(lk, [this]()
        {
            return !_commands.empty();
        });

    std::string cmd{ _commands.front() };
    _commands.pop_front();

    return cmd;
}

//-----------------------------------------------------------------------------

void QtWindow::reply(const std::string_view resp)
{
    if (SUCCESS_RESP == resp) {
        showMessage(SUCCESS_RESP);
    }
    else if (FAILURE_RESP == resp) {
        showMessage("Failure");
    }
    else {
        QString value(resp.data());

        _ui->valueEdit->setText(value);
    }
}

//-----------------------------------------------------------------------------

void QtWindow::show()
{
    QMainWindow::show();
}

//-----------------------------------------------------------------------------

void QtWindow::sendCommand(const std::string &cmd)
{
    assert(!cmd.empty());

    _ui->keyEdit->clear();
    _ui->valueEdit->clear();

    std::lock_guard<std::mutex> lk{_commands_lock};

    _commands.push_back(cmd);

    _commands_cond.notify_one();
}

//-----------------------------------------------------------------------------

void QtWindow::showMessage(const QString &msg)
{
    assert(!msg.isEmpty());

    constexpr const int timeout_ms{ 0 };

    _ui->statusbar->showMessage(msg, timeout_ms);
}

//-----------------------------------------------------------------------------

void QtWindow::closeEvent(QCloseEvent *)
{
    sendCommand(EXIT_CMD);
}

//-----------------------------------------------------------------------------

void QtWindow::getClicked()
{
    // TODO
    showMessage("Loading value from key...");

    std::string key{ _ui->keyEdit->text().toStdString() };
    std::string cmd{ GET_CMD };
    cmd += " " + key;

    sendCommand(cmd);
}

//-----------------------------------------------------------------------------

void QtWindow::setClicked()
{
    // TODO
    showMessage("Saving key and value...");

    std::string key{ _ui->keyEdit->text().toStdString() };
    std::string value{ _ui->valueEdit->text().toStdString() };
    std::string cmd{ SET_CMD };
    cmd += " " + key + " " + value;

    sendCommand(cmd);
}

//-----------------------------------------------------------------------------

void QtWindow::delClicked()
{
    // TODO
    showMessage("Deleting key and value...");

    std::string key{ _ui->keyEdit->text().toStdString() };
    std::string cmd{ DELETE_CMD };
    cmd += " " + key;

   sendCommand(cmd);
}

//-----------------------------------------------------------------------------
