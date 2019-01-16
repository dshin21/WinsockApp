#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  public slots:
    void host_to_ip();
    void ip_to_host();
    void port_to_service();
    void service_to_port();

  private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
