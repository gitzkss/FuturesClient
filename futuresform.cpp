#include "futuresform.h"
#include "ui_futuresform.h"

FuturesForm::FuturesForm(QTcpSocket *s, std::string id, QWidget *parent) :
    QWidget(parent),ui(new Ui::FuturesForm)
{
    ui->setupUi(this);
    customerID = id;
    socket = s;
    // 连接信号和槽函数
    connect(socket, &QTcpSocket::connected, this, &FuturesForm::socketConnected);
    connect(socket, &QTcpSocket::disconnected, this, &FuturesForm::socketDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &FuturesForm::socketRead);
    getPanKou();

}

FuturesForm::~FuturesForm()
{
    delete ui;
}

void FuturesForm::getPanKou()
{
    std::map<std::string,std::string> send;
    send["op"]="pankou";
    //请求盘口
    socket->write(serializeMap(send).c_str());
}

void FuturesForm::on_mairuButton_clicked()
{
    std::string type;
    if(ui->kaicangRadio->isChecked())
    {
        type="duokai";
        WeiTuo wt;
        wt.setCustomerID(QString::fromStdString(customerID).toInt());
        wt.setCount(ui->countSpinBox->text().toInt());
        wt.setOptype(type);
        wt.setPrice(ui->priceSpinBox->text().toDouble());
        std::string send = Weituo_to_str(wt);
        socket->write(send.c_str());
    }
    else if(ui->pingcangRadio->isChecked())
    {
        type="duoping";
        WeiTuo wt;
        wt.setCustomerID(QString::fromStdString(customerID).toInt());
        wt.setCount(ui->countSpinBox->text().toInt());
        wt.setOptype(type);
        wt.setPrice(ui->priceSpinBox->text().toDouble());
        std::string send = Weituo_to_str(wt);
        socket->write(send.c_str());
    }
    else
    {
        QMessageBox::information(this, "错误", "请选择开仓或平仓！");
    }
}


void FuturesForm::socketConnected()
{

}

void FuturesForm::socketRead()
{
    std::string data = socket->readAll().toStdString();
    bug(data);
    std::map<std::string,std::string> d = deserializeMap(data);
    if(d["type"]=="pankou")
    {
        updatePanKou(d);
    }
    else if(d["type"]=="weituo")
    {
        if(d["result"]=="error")
            QMessageBox::information(this, "错误", QString::fromStdString(d["msg"]));
    }
}

void FuturesForm::updatePanKou(std::map<std::string,std::string>& d)
{
    int buyPrice = (int)std::stoi(d["buyPrice"]);
    int sellPrice = (int)std::stoi(d["sellPrice"]);
    int newPrice = (int)std::stoi(d["newPrice"]);
    int zhangTingPrice = (int)std::stoi(d["zhangTingPrice"]);
    int dieTingPrice = (int)std::stoi(d["dieTingPrice"]);
    if(buyPrice==-1)
        ui->buyPrice->setText("---");
    else
        ui->buyPrice->setText(QString::fromStdString(std::to_string(buyPrice)));
    if(sellPrice==-1)
         ui->sellPrice->setText("---");
    else
         ui->sellPrice->setText(QString::fromStdString(std::to_string(sellPrice)));
    ui->buyCount->setText(QString::fromStdString(d["buyCount"]));
    ui->sellCount->setText(QString::fromStdString(d["sellCount"]));
    ui->zuixin->setText(QString::fromStdString(std::to_string(newPrice)));
    ui->zhangting->setText(QString::fromStdString(std::to_string(zhangTingPrice)));
    ui->dieting->setText(QString::fromStdString(std::to_string(dieTingPrice)));
    ui->neipan->setText(QString::fromStdString(d["neiPan"]));
    ui->waipan->setText(QString::fromStdString(d["waiPan"]));
    ui->fudu->setText(QString::fromStdString(d["zhangDieFu"].substr(0,4)+"%"));

}

void FuturesForm::socketDisconnected()
{
    //QMessageBox::information(this, "Disconnected", "Disconnected from the server.");
}


void FuturesForm::on_maichuButton_clicked()
{
    std::string type;
    if(ui->kaicangRadio->isChecked())
    {
        type="kongkai";
        WeiTuo wt;
        wt.setCustomerID(QString::fromStdString(customerID).toInt());
        wt.setCount(ui->countSpinBox->text().toInt());
        wt.setOptype(type);
        wt.setPrice(ui->priceSpinBox->text().toDouble());
        std::string send = Weituo_to_str(wt);
        socket->write(send.c_str());
    }
    else if(ui->pingcangRadio->isChecked())
    {
        type="kongping";
        WeiTuo wt;
        wt.setCustomerID(QString::fromStdString(customerID).toInt());
        wt.setCount(ui->countSpinBox->text().toInt());
        wt.setOptype(type);
        wt.setPrice(ui->priceSpinBox->text().toDouble());
        std::string send = Weituo_to_str(wt);
        socket->write(send.c_str());
    }
    else
    {
        QMessageBox::information(this, "错误", "请选择开仓或平仓！");
    }
}

