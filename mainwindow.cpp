#include "mainwindow.h"
#include "ui_mainwindow.h"
//TODO: get rid of this
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btn_host_to_ip, &QAbstractButton::clicked, this, &MainWindow::host_to_ip);
    connect(ui->btn_ip_to_host, &QAbstractButton::clicked, this, &MainWindow::ip_to_host);
    connect(ui->btn_port_to_service, &QAbstractButton::clicked, this, &MainWindow::port_to_service);
    connect(ui->btn_service_to_port, &QAbstractButton::clicked, this, &MainWindow::service_to_port);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::host_to_ip()
{
    qDebug() << "hi1";
}

void MainWindow::ip_to_host()
{
    qDebug() << "hi2";
}

void MainWindow::port_to_service()
{
    qDebug() << "hi3";
}

void MainWindow::service_to_port()
{
    qDebug() << "hi4";
}
