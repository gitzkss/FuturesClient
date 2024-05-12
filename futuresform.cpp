#include "futuresform.h"
#include "ui_futuresform.h"

FuturesForm::FuturesForm(QTcpSocket *s, std::string id, QWidget *parent) :
    QWidget(parent),ui(new Ui::FuturesForm)
{
    ui->setupUi(this);
    customerID = id;
    ui->customerID->setText(QString::fromStdString(customerID));
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
    weituoButton = {
        ui->weituo_1,
        ui->weituo_2,
        ui->weituo_3,
        ui->weituo_4,
        ui->weituo_5,
        ui->weituo_6,
        ui->weituo_7,
        ui->weituo_8,
        ui->weituo_9,
        ui->weituo_10
    };
    priceKeys = {"price1", "price2", "price3", "price4", "price5",
                                          "price6", "price7", "price8", "price9", "price10"};

    countKeys = {"count1", "count2", "count3", "count4", "count5",
                                          "count6", "count7", "count8", "count9", "count10"};

    optypeKeys = {"optype1", "optype2", "optype3", "optype4", "optype5",
                                           "optype6", "optype7", "optype8", "optype9", "optype10"};
    colorKeys = {"color1", "color2", "color3", "color4", "color5",
                                      "color6", "color7", "color8", "color9", "color10"};
    // 创建信号映射器
    signalMapper = new QSignalMapper(this);

    // 将每个按钮连接到 Lambda 表达式槽函数
    for (int i = 0; i < 10; ++i)
    {
        QPushButton *button = weituoButton[i];
        connect(button, &QPushButton::clicked, [this, i](){ this->cancel_weituo_clicked(i + 1); });
    }

    //进度条
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(1000);

    // 设置未完成部分的颜色（卖一价数量）为绿色
    ui->progressBar->setStyleSheet("QProgressBar {border-radius:1px; background:#00ca00;}"
                                   "QProgressBar::chunk { background: red; }");
    ui->dieting->setStyleSheet("color: #00ca00;");
    ui->neipan->setStyleSheet("color: #00ca00;");
    ui->waipan->setStyleSheet("color: red;");
    ui->zhangting->setStyleSheet("color: red;");

}

FuturesForm::~FuturesForm()
{
    delete ui;
}

void FuturesForm::cancel_weituo_clicked(int i)
{
    if(weituolist[i].getOptype()!="")
    {
        std::string send = Weituo_to_str(weituolist[i]);
        socket->write(send.c_str());
    }
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

void FuturesForm::updateInfo(std::map<std::string,std::string> d)
{
    //bug2("+++",d["keyong"]);
    std::string quanyi = d["quanyi"].substr(0,d["quanyi"].find('.')+3);
    std::string keyong = d["keyong"].substr(0,d["keyong"].find('.')+3);
    ui->quanyi->setText(QString::fromStdString(quanyi));
    ui->keyong->setText(QString::fromStdString(keyong));
    ui->duodan->setText(QString::fromStdString(d["duo"]));
    ui->kongdan->setText(QString::fromStdString(d["kong"]));
    double fengxianlv = std::stod(d["fengxianlv"]);
    std::string avgduo = d["avgduo"].substr(0,d["avgduo"].find('.')+2);
    std::string avgkong = d["avgkong"].substr(0,d["avgkong"].find('.')+2);
    fengxianlv *= 100;
    std::string fxl = std::to_string(fengxianlv).substr(0,5)+"%";
    ui->fengxianlv->setText(QString::fromStdString(fxl));
    ui->avgduo->setText(QString::fromStdString(avgduo));
    ui->avgkong->setText(QString::fromStdString(avgkong));
    double fudongduo = (std::stod(d["newPrice"])-std::stod(avgduo))*std::stoi(d["duo"]);
    double fudongkong = (std::stod(avgkong)-std::stod(d["newPrice"]))*std::stoi(d["kong"]);
    double fudongyingkui = fudongduo+fudongkong;
    std::string sfudongyingkui = std::to_string(fudongyingkui);
    sfudongyingkui = sfudongyingkui.substr(0,sfudongyingkui.find('.')+2);
    ui->fudongyingkui->setText(QString::fromStdString(sfudongyingkui));
    if(std::stoi(d["duo"])!=0||std::stoi(d["kong"])!=0)
    {
        if(fudongyingkui>=0)
            ui->fudongyingkui->setStyleSheet("color: red;");
        else
            ui->fudongyingkui->setStyleSheet("color: #00ca00;");
    }
    else
        ui->fudongyingkui->setStyleSheet("color: black;");
    std::string msg;
    for(int i = 0; i < 10; i++)
    {
        if(d["_count"+std::to_string(i+1)]=="")
        {
            weituoButton[i]->setText("空");
            weituoButton[i]->setStyleSheet("color: black;");
            weituolist[i+1].setOptype("");
            continue;
        }
        weituolist[i+1].setCustomerID(std::stoi(customerID));
        weituolist[i+1].setOptype(d["_optype"+std::to_string(i+1)]);
        weituolist[i+1].setCount(-std::stoi(d["_count"+std::to_string(i+1)]));
        weituolist[i+1].setPrice(std::stod(d["_price"+std::to_string(i+1)]));

        msg += modifyOptype(d["_optype"+std::to_string(i+1)])+" ";
        msg += d["_count"+std::to_string(i+1)]+"手 ";
        msg += d["_price"+std::to_string(i+1)];
        weituoButton[i]->setText(QString::fromStdString(msg));
        //bug(d["_optype"+std::to_string(i+1)]);
        if((d["_optype"+std::to_string(i+1)]=="duokai")||(d["_optype"+std::to_string(i+1)]=="kongping"))
            weituoButton[i]->setStyleSheet("color: red;");
        else
            weituoButton[i]->setStyleSheet("color: #00ca00;");
        msg="";
    }
}

void FuturesForm::socketRead()
{
    std::string data = socket->readAll().toStdString();
    std::map<std::string,std::string> d = deserializeMap(data);
    if(d["type"]=="pankou")
    {
        bug(data);
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

std::string FuturesForm::modifyOptype(std::string op)
{
    if(op=="duokai")
        op="多开";
    else if(op=="duoping")
        op="多平";
    else if(op=="kongkai")
        op="空开";
    else if(op=="kongping")
        op="空平";
    else if(op=="shuangkai")
        op="双开";
    else if(op=="shuangping")
        op="双平";
    else if(op=="duohuan")
        op="多换";
    else if(op=="konghuan")
        op="空换";
    return op;
}

void FuturesForm::updatePanKou(std::map<std::string,std::string> d)
{
    updateInfo(d);
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
    ui->shouxufei->setText(QString::fromStdString(d["shouxufei"]));
    ui->ganggan->setText(QString::fromStdString(d["ganggan"]+" 倍"));
    //设置涨跌幅颜色
    if(d["zhangDieFu"][0]=='-')
         ui->fudu->setStyleSheet("color: #00ca00;");
    else
         ui->fudu->setStyleSheet("color: red;");
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
         it->second = modifyOptype(it->second);
    }

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
         if(d[colorKeys[i]]=="1")
            ChengJiaolabels[i][2]->setStyleSheet("color: red;");
         else
            ChengJiaolabels[i][2]->setStyleSheet("color: #00ca00;");

    }

    for (int i = 0; i < maxcnt; ++i)
    {
         ChengJiaolabels[i][3]->setText(QString::fromStdString(d[optypeKeys[i]]));
         if(d[colorKeys[i]]=="1")
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


