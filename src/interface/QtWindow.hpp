#ifndef QT_WINDOW_HPP_INCLUDED
#define QT_WINDOW_HPP_INCLUDED

#include <condition_variable>
#include <list>
#include <mutex>
#include <string>
#include <string_view>

#include <QMainWindow>
#include <QString>

#include "Window.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class QtWindow; }
QT_END_NAMESPACE

/**
 * Graphical User Window
 */
class QtWindow : public QMainWindow, public Window
{
    Q_OBJECT

public:
    explicit QtWindow(QWidget *parent = nullptr);
    QtWindow(const QtWindow &) = delete;
    virtual ~QtWindow();

    QtWindow operator=(const QtWindow &) = delete;

    virtual std::string request() override;
    virtual void reply(const std::string_view resp) override;
    virtual void show() override;

    void sendCommand(const std::string &cmd);
    void showMessage(const QString &msg);
    void closeEvent(QCloseEvent *event) override;

public slots:
    void getClicked();
    void setClicked();
    void delClicked();

private:
    Ui::QtWindow *_ui;
    std::list<std::string> _commands;
    std::mutex _commands_lock;
    std::condition_variable _commands_cond;

};

#endif // QT_WINDOW_HPP_INCLUDED
