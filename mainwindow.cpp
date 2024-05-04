#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), socket(nullptr)
{
    ui->setupUi(this);

    //ip = "192.168.137.128";
    ip = "127.0.0.1";
    port = "8888";

    // 初始化 socket 对象
    socket = new QTcpSocket(this);

    is_login = false;
    // 连接信号和槽函数
    connect(socket, &QTcpSocket::connected, this, &MainWindow::socketConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketRead);
}

MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    socket->abort();
    socket->connectToHost(ip, port.toShort());
}

void MainWindow::socketConnected()
{
    socket->write(login_info(ui->IDInput->text().toStdString(),ui->passwordInput->text().toStdString()).c_str());
}

void MainWindow::socketRead()
{

    std::string data = socket->readAll().toStdString();
    std::map<std::string,std::string> d = deserializeMap(data);
    bug(data);
    if(d["result"]=="ok")
    {
        this->hide();
        FuturesForm *futuresForm = new FuturesForm(socket,ui->IDInput->text().toStdString());
        futuresForm->show();
        disconnect(socket, &QTcpSocket::connected, this, &MainWindow::socketConnected);
        disconnect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
        disconnect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketRead);
        is_login = true;
    }
    else
    {
        QString msg = QString::fromStdString(d["msg"]);
        QMessageBox::information(this, "登录失败", msg);
    }
}

void MainWindow::socketDisconnected()
{
    //QMessageBox::information(this, "Disconnected", "Disconnected from the server.");
}
