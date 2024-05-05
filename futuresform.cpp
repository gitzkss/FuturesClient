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

    ChengJiaolabels = {
        {ui->time_1, ui->price_1, ui->count_1, ui->type_1},
        {ui->time_2, ui->price_2, ui->count_2, ui->type_2},
        {ui->time_3, ui->price_3, ui->count_3, ui->type_3},
        {ui->time_4, ui->price_4, ui->count_4, ui->type_4},
        {ui->time_5, ui->price_5, ui->count_5, ui->type_5},
        {ui->time_6, ui->price_6, ui->count_6, ui->type_6},
        {ui->time_7, ui->price_7, ui->count_7, ui->type_7},
        {ui->time_8, ui->price_8, ui->count_8, ui->type_8},
        {ui->time_9, ui->price_9, ui->count_9, ui->type_9},
        {ui->time_10, ui->price_10, ui->count_10, ui->type_10}
    };

    //进度条
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(1000);

    // 设置未完成部分的颜色（卖一价数量）为绿色
    ui->progressBar->setStyleSheet("QProgressBar {border-radius:1px; background:#00ca00;}"
                                   "QProgressBar::chunk { background: red; }");



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

std::string FuturesForm::time_t_to_string(const std::string& timeString) {
    if(timeString=="")
        return "";
    // 将输入的字符串直接转换为 time_t 类型
    time_t timestamp = std::stoll(timeString);

    // 转换为本地时间结构
    tm* localTime = localtime(&timestamp);

    // 提取小时和分钟
    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;
    // 手动构建字符串
    std::string hourStr = hour < 10 ? "0" + std::to_string(hour) : std::to_string(hour);
    std::string minuteStr = minute < 10 ? "0" + std::to_string(minute) : std::to_string(minute);
    std::string secondStr = (second < 10) ? "0" + std::to_string(second) : std::to_string(second);
    return hourStr + ":" + minuteStr + ":" + secondStr;
}

void FuturesForm::updatePanKou(std::map<std::string,std::string>& d)
{
    int buyPrice = (int)std::stoi(d["buyPrice"]);
    int sellPrice = (int)std::stoi(d["sellPrice"]);
    int zuigao = (int)std::stoi(d["maxPrice"]);
    int zuidi = (int)std::stoi(d["minPrice"]);
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
    if(zuigao==-1)
         ui->zuigao->setText("---");
    else
         ui->zuigao->setText(QString::fromStdString(std::to_string(zuigao)));
    if(zuidi==0x3f3f3f3f)
         ui->zuidi->setText("---");
    else
         ui->zuidi->setText(QString::fromStdString(std::to_string(zuidi)));

    ui->buyCount->setText(QString::fromStdString(d["buyCount"]));
    ui->sellCount->setText(QString::fromStdString(d["sellCount"]));
    ui->zuixin->setText(QString::fromStdString(std::to_string(newPrice)));
    ui->zhangting->setText(QString::fromStdString(std::to_string(zhangTingPrice)));
    ui->dieting->setText(QString::fromStdString(std::to_string(dieTingPrice)));
    ui->neipan->setText(QString::fromStdString(d["neiPan"]));
    ui->waipan->setText(QString::fromStdString(d["waiPan"]));
    ui->fudu->setText(QString::fromStdString(d["zhangDieFu"].substr(0,4)+"%"));

    // 买一价数量和卖一价数量
    int buyCount = QString::fromStdString(d["buyCount"]).toInt();
    int sellCount = QString::fromStdString(d["sellCount"]).toInt();

    // 计算比例
    double total = buyCount + sellCount;
    double buyPercentage = buyCount / total;


    // 设置进度条的比例
    ui->progressBar->setValue(buyPercentage * 1000); // 买一价数量在左侧
    ui->progressBar->setAlignment(Qt::AlignLeft);
    ui->progressBar->setAlignment(Qt::AlignRight);


    for(auto it = d.begin();it!=d.end();it++)
    {
         if(it->second=="duokai")
            it->second="多开";
         else if(it->second=="duoping")
            it->second="多平";
         else if(it->second=="kongkai")
            it->second="空开";
         else if(it->second=="kongping")
            it->second="空平";
         else if(it->second=="shuangkai")
            it->second="双开";
         else if(it->second=="shuangping")
            it->second="双平";
         else if(it->second=="duohuan")
            it->second="多换";
         else if(it->second=="konghuan")
            it->second="空换";
    }
    std::vector<std::string> priceKeys = {"price1", "price2", "price3", "price4", "price5",
                                         "price6", "price7", "price8", "price9", "price10"};

    std::vector<std::string> countKeys = {"count1", "count2", "count3", "count4", "count5",
                                         "count6", "count7", "count8", "count9", "count10"};

    std::vector<std::string> optypeKeys = {"optype1", "optype2", "optype3", "optype4", "optype5",
                                          "optype6", "optype7", "optype8", "optype9", "optype10"};
    std::vector<std::string> color = {"color1", "color2", "color3", "color4", "color5",
                                           "color6", "color7", "color8", "color9", "color10"};
    int maxcnt = 10;
    for (int i = 0; i < maxcnt; ++i)
    {
         QString timeStr = QString::fromStdString(time_t_to_string(d["time" + std::to_string(i + 1)]));
         ChengJiaolabels[i][0]->setText(timeStr);
    }

    for (int i = 0; i < maxcnt; ++i)
    {
         ChengJiaolabels[i][1]->setText(QString::fromStdString(d[priceKeys[i]]));
    }

    for (int i = 0; i < maxcnt; ++i)
    {
         ChengJiaolabels[i][2]->setText(QString::fromStdString(d[countKeys[i]]));
         if(d[color[i]]=="1")
            ChengJiaolabels[i][2]->setStyleSheet("color: red;");
         else
            ChengJiaolabels[i][2]->setStyleSheet("color: #00ca00;");

    }

    for (int i = 0; i < maxcnt; ++i)
    {
         ChengJiaolabels[i][3]->setText(QString::fromStdString(d[optypeKeys[i]]));
         if(d[color[i]]=="1")
            ChengJiaolabels[i][3]->setStyleSheet("color: red;");
         else
            ChengJiaolabels[i][3]->setStyleSheet("color: #00ca00;");
    }

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

