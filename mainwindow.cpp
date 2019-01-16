#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
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
    while (true)
    {
        int a;
        struct hostent *hp;
        struct in_addr my_addr, *addr_p;
        char **p;
        char ip_address[256];

        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;

        WSAStartup(wVersionRequested, &wsaData);

        addr_p = (struct in_addr *)malloc(sizeof(struct in_addr));
        addr_p = &my_addr;

        QString arg1 = ui->textbox_input->text();
        QString result = "";

        addr_p = (struct in_addr *)malloc(sizeof(struct in_addr));
        addr_p = &my_addr;

        if ((hp = gethostbyname(arg1.toStdString().c_str())) == NULL)
        {
            switch (h_errno)
            {
            case HOST_NOT_FOUND:
                result.append("No such host " + arg1 + "\n");
                break;
            case TRY_AGAIN:
                result.append("host " + arg1 + " try again later\n");
                break;
            case NO_RECOVERY:
                result.append("host " + arg1 + " DNS Error\n");
                break;
            case NO_ADDRESS:
                result.append("No IP Address for " + arg1 + "\n");
                break;
            default:
                result.append("Unknown Error:  " + QString::number(h_errno) + "\n");
                break;
            }
        }
        else
        {
            for (p = hp->h_addr_list; *p != 0; p++)
            {
                struct in_addr in;
                char **q;

                memcpy(&in.s_addr, *p, sizeof(in.s_addr));
                result.append("IP Address: " + QString(inet_ntoa(in)) + "\nHost Name: " + QString(hp->h_name) + "\n");

                for (q = hp->h_aliases; *q != 0; q++)
                {
                    result.append("\t   Aliases: " + QString(*q) + "\n");
                }
                result.append('\n');
            }
        }
        ui->textbox_output->setText(result);
        WSACleanup();
        break;
    }
}

void MainWindow::ip_to_host()
{
    while (true)
    {
        int a;
        struct hostent *hp;
        struct in_addr my_addr, *addr_p;
        char **p;
        char ip_address[256];

        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;

        WSAStartup(wVersionRequested, &wsaData);

        addr_p = (struct in_addr *)malloc(sizeof(struct in_addr));
        addr_p = &my_addr;

        QString arg1 = ui->textbox_input->text();
        QString result = "";

        if ((a = inet_addr(arg1.toStdString().c_str())) == 0)
        {
            result = ("IP Address must be of the form x.x.x.x\n");
            break;
        }

        strcpy(ip_address, arg1.toStdString().c_str());
        addr_p->s_addr = inet_addr(ip_address);

        hp = gethostbyaddr((char *)addr_p, PF_INET, sizeof(my_addr));

        if (hp == NULL)
        {
            result = "host information for " + arg1 + " not found";
            result.append('\n');
            break;
        }

        for (p = hp->h_addr_list; *p != 0; p++)
        {
            struct in_addr in;
            char **q;

            memcpy(&in.s_addr, *p, sizeof(in.s_addr));
            result.append("IP Address: " + QString(inet_ntoa(in)) + "\nHost Name: " + hp->h_name);
            result.append('\n');
            result.append("Aliases: \n");
            for (q = hp->h_aliases; *q != 0; q++)
            {
                result.append("\t" + QString(*q) + "\n");
            }
            result.append('\n');
        }
        ui->textbox_output->setText(result);
        WSACleanup();
        break;
    }
}

void MainWindow::port_to_service()
{
    while (true)
    {
        struct servent *sv;
        int s_port;

        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        WSAStartup(wVersionRequested, &wsaData);
        QStringList input = ui->textbox_input->text().split(" ");
        QString arg1 = input.at(0);
        QString arg2 = input.at(1);
        QString result = "";

        s_port = atoi(arg1.toStdString().c_str());

        sv = getservbyport(htons(s_port), arg2.toStdString().c_str());
        if (sv == NULL)
        {
            result = "An error has occurred with service from port";
        }
        else
        {
            result = "The service for " + arg2 + " port " + QString::number(s_port) + " is " + sv->s_name;
        }
        result.append('\n');
        ui->textbox_output->setText(result);
        WSACleanup();
        break;
    }
}

void MainWindow::service_to_port()
{
    while (true)
    {
        struct servent *sv;

        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        WSAStartup(wVersionRequested, &wsaData);
        QStringList input = ui->textbox_input->text().split(" ");
        QString arg1 = input.at(0);
        QString arg2 = input.at(1);
        QString result = "";

        sv = getservbyname(arg1.toStdString().c_str(), arg2.toStdString().c_str());
        if (sv == NULL)
        {
            result = "An error has occurred with port from service";
        }
        else
        {
            result = "The port number for " + arg1 + " is ";
            result.append(QString::number(ntohs(sv->s_port)));
        }
        result.append('\n');
        ui->textbox_output->setText(result);
        WSACleanup();
        break;
    }
}
