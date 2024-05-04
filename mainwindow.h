#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include<QHostAddress>
#include<QMessageBox>
#include"futuresform.h"
#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void socketConnected();
    void socketDisconnected();
    void on_loginButton_clicked();
    void socketRead();

private:
    bool is_login;
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString ip;
    QString port;
};
#endif // MAINWINDOW_H
