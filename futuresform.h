#ifndef FUTURESFORM_H
#define FUTURESFORM_H

#include <QWidget>
#include "utils.h"
#include "wei_tuo.h"
#include "utils.h"
#include <string>
#include "wei_tuo.h"
#include <qtcpsocket.h>
#include <QMessageBox>
#include "utils.h"
#include <QSignalMapper>

namespace Ui {
class FuturesForm;
}

class FuturesForm : public QWidget
{
    Q_OBJECT

public:
    explicit FuturesForm(QTcpSocket *s, std::string customerID, QWidget *parent = nullptr);
    void getPanKou();
    std::string time_t_to_string(const std::string& timeString);
    void updatePanKou(std::map<std::string,std::string> d);
    void updateInfo(std::map<std::string,std::string> d);
    std::string modifyOptype(std::string op);
    ~FuturesForm();

private slots:
    void on_mairuButton_clicked();
    void socketConnected();
    void socketDisconnected();
    void socketRead();

    void on_maichuButton_clicked();
    void cancel_weituo_clicked(int i);

private:
    Ui::FuturesForm *ui;
    QTcpSocket *socket;
    std::string customerID;
    std::vector<std::vector<QLabel*>> ChengJiaolabels;
    std::vector<QPushButton*> weituoButton;
    WeiTuo weituolist[11];
    QSignalMapper *signalMapper; // 定义信号映射器
    std::vector<std::string> priceKeys;
    std::vector<std::string> countKeys;
    std::vector<std::string> optypeKeys;
    std::vector<std::string> colorKeys;

};

#endif // FUTURESFORM_H
