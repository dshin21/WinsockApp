/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: mainwindow.cpp - A windows program that performs TCP/IP lookup functions.
--
-- PROGRAM:     WinsockApp
--
-- FUNCTIONS:
--              void host_to_ip()
--              void ip_to_host()
--              void port_to_service()
--              void service_to_port()
--              void cleanup()
--              bool validate_user_input(QString)
--              void print_to_console(QString)
--
-- DATE:        Jan. 16, 2019
--
-- REVISIONS:   None
--
-- DESIGNER:    Daniel Shin
--
-- PROGRAMMER:  Daniel Shin
--
-- NOTES:
--              This program performs four TCP/IP lookup functions using the Winsock2 API:
--                  - take a user specified host name and resolve it into a IP address
--                  - take a user specified IP address and resolve it into host name(s)
--                  - take a user specified service name/protocol and resolve it into its port number
--                  - take a user specified port number/protocol and resolve it into its service name
----------------------------------------------------------------------------------------------------------------------*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:    host_to_ip
--
-- DATE:        Jan. 16, 2019
--
-- DESIGNER:    Daniel Shin
--
-- PROGRAMMER:  Daniel Shin
--
-- INTERFACE:   void host_to_ip()
--
-- RETURNS:     void
--
-- NOTES:
--              This function is responsible for using the Winsock v2.2 session and searching
--              to see if the IP address exists, given an host name.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::host_to_ip()
{
    while (true)
    {
        QString arg1 = ui->textbox_input->text();
        QString result = "";

        if (!validate_user_input(arg1))
        {
            ui->textbox_output->setText("Please Enter a valid host name.");
            cleanup();
            break;
        }

        struct hostent *hp;
        struct in_addr my_addr, *addr_p;
        char **p;

        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;

        WSAStartup(wVersionRequested, &wsaData);

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
                    result.append("Aliases: " + QString(*q) + "\n");
                }
            }
        }

        print_to_console(result);
        cleanup();
        break;
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:    ip_to_host
--
-- DATE:        Jan. 16, 2019
--
-- DESIGNER:    Daniel Shin
--
-- PROGRAMMER:  Daniel Shin
--
-- INTERFACE:   void ip_to_host()
--
-- RETURNS:     void
--
-- NOTES:
--              This function is responsible for using the Winsock v2.2 session and searching
--              to see if the host exists, given an IP address.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::ip_to_host()
{
    while (true)
    {
        QString arg1 = ui->textbox_input->text();

        if (!validate_user_input(arg1))
        {
            ui->textbox_output->setText("Please Enter in the form of:\nx.x.x.x");
            cleanup();
            break;
        }

        int a;
        struct hostent *hp;
        struct in_addr my_addr, *addr_p;
        char **p;
        char ip_address[256];

        QString result = "";

        if ((a = inet_addr(arg1.toStdString().c_str())) == 0)
        {
            result = ("IP Address must be of the form x.x.x.x\n");
            break;
        }
        
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;

        WSAStartup(wVersionRequested, &wsaData);

        addr_p = (struct in_addr *)malloc(sizeof(struct in_addr));
        addr_p = &my_addr;
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
                result.append(QString(*q) + "\n");
            }
        }

        print_to_console(result);
        cleanup();
        break;
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:    port_to_service
--
-- DATE:        Jan. 16, 2019
--
-- DESIGNER:    Daniel Shin
--
-- PROGRAMMER:  Daniel Shin
--
-- INTERFACE:   void port_to_service()
--
-- RETURNS:     void
--
-- NOTES:
--              This function is responsible for using the Winsock v2.2 session and searching
--              to see if the service exists, given the port/protocol combination.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::port_to_service()
{
    while (true)
    {
        if (!validate_user_input(ui->textbox_input->text()))
        {
            ui->textbox_output->setText("Please Enter in the form of:\nport protocol");
            cleanup();
            break;
        }

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
        print_to_console(result);
        cleanup();
        break;
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:    service_to_port
--
-- DATE:        Jan. 16, 2019
--
-- DESIGNER:    Daniel Shin
--
-- PROGRAMMER:  Daniel Shin
--
-- INTERFACE:   void service_to_port()
--
-- RETURNS:     void
--
-- NOTES:
--              This function is responsible for using the Winsock v2.2 session and searching
--              to see if the port exists, given the service/protocol combination.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::service_to_port()
{
    while (true)
    {
        if (!validate_user_input(ui->textbox_input->text()))
        {
            ui->textbox_output->setText("Please Enter in the form of:\nservice protocol");
            cleanup();
            break;
        }

        struct servent *sv;

        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        WSAStartup(wVersionRequested, &wsaData);
        QString result = "";

        QStringList input = ui->textbox_input->text().split(" ");
        QString arg1 = input.at(0);
        QString arg2 = input.at(1);

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

        print_to_console(result);
        cleanup();
        break;
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:    cleanup
--
-- DATE:        Jan. 16, 2019
--
-- DESIGNER:    Daniel Shin
--
-- PROGRAMMER:  Daniel Shin
--
-- INTERFACE:   void cleanup()
--
-- RETURNS:     void
--
-- NOTES:
--              This function clears the INPUT textbox after the user selects a button 
--              and calls WSACleanup() close the Winsock session.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::cleanup()
{
    ui->textbox_input->clear();
    WSACleanup();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:    validate_user_input
--
-- DATE:        Jan. 16, 2019
--
-- DESIGNER:    Daniel Shin
--
-- PROGRAMMER:  Daniel Shin
--
-- INTERFACE:   void validate_user_input(QString input)
--
-- RETURNS:     bool - returns true if user provides an input, false if no input
--
-- NOTES:
--              This function validates to see if the user inputs a value into the
--              INPUT textbox.
----------------------------------------------------------------------------------------------------------------------*/
bool MainWindow::validate_user_input(QString input)
{
    if (input.length() == 0)
    {
        return false;
    }

    return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:    print_to_console
--
-- DATE:        Jan. 16, 2019
--
-- DESIGNER:    Daniel Shin
--
-- PROGRAMMER:  Daniel Shin
--
-- INTERFACE:   void print_to_console(QString result)
--
-- RETURNS:     void
--
-- NOTES:
--              This function prints the result (either an error message or the searched result)
--              to the OUTPUT textbox.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::print_to_console(QString result)
{
    ui->textbox_output->setText(result);
}
