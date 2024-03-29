
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QHostInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpClient;
    QString ipadress ;
    QHostAddress hostaddress;
    int PID ;


    QByteArray getIPtoUtf8();
    QByteArray getPorttoUtf8();
};

#endif // MAINWINDOW_H
