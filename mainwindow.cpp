#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SETSTART tcpClient->write("%");
#define SETPOINT tcpClient->write("|");
#define SETEND tcpClient->write("&");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->ipAddress->setText("192.168.1.2");
    ui->ipAddress->setText("127.0.0.1");
    ui->Port->setText("55555");

    tcpClient = new QTcpSocket(this);
    tcpClient->abort();
    connect(ui->Ok,&QAbstractButton::clicked,[=]{
        PID = ui->pidEdit->text().toInt();
        QString ip_temp = ui->ipAddress->text();

        ui->textBrowser->append(ip_temp);
        unsigned short port = ui->Port->text().toUShort();

        tcpClient->connectToHost(ip_temp,port,QTcpSocket::ReadWrite);
        tcpClient->waitForConnected();

        if(tcpClient->state()==QTcpSocket::ConnectedState)
        {
            ui->Ok->setDisabled(true);
            ui->textBrowser->append(QString::number(port));
            ui->textBrowser->append("connected!");
            //send PID
            tcpClient->write(QString::number(PID).toUtf8());
        }
    });

    connect(ui->Ok,&QAbstractButton::clicked,[=]{
        connect(tcpClient,&QTcpSocket::readyRead,
                [=]{
            QByteArray temp = tcpClient->readAll();
            if(temp == "GET_DATA")
            {
                SETSTART
                tcpClient->write(QString::number(PID).toUtf8());
                SETPOINT
                tcpClient->write(getIPtoUtf8());
                SETPOINT
                tcpClient->write(getPorttoUtf8());
                SETPOINT
                tcpClient->write("connecting");
                SETEND
            }
            else
                ui->textBrowser->append(temp);
            temp.clear();
        });
    });

    connect(ui->cclean,&QAbstractButton::clicked,[=]{
        this->close();
    });
}

MainWindow::~MainWindow()
{
    tcpClient->write(QString::number(PID).toUtf8());
    delete ui;
}

QByteArray MainWindow::getIPtoUtf8()
{
    QHostAddress myip = tcpClient->peerAddress();
    return myip.toString().toUtf8();
}

QByteArray MainWindow::getPorttoUtf8()
{
    unsigned short myport = tcpClient->peerPort();
    return QString::number(myport).toUtf8();
}
