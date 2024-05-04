#pragma once
#include<ctime>
#include<string>
class WeiTuo
{
private:
    //下单时间
    time_t time;
    //委托id
    int id;
    //客户id
    int customerID;
    //合约id
    int contractID;
    //合约名称
    std::string contractName;
    //操作类型
    std::string optype;
    //下单价格
    double price;
    //下单数量  负数为撤单标记
    int count;
public:
    WeiTuo(time_t t = -1, int id = -1, int customerID = -1, int contractID = -1, std::string contractName = "", std::string optype = "", double price = -1, int count = -1);
    ~WeiTuo();
    time_t getTime();
    int getID();
    int getCustomerID();
    int getContractID();
    std::string getContractName();
    std::string getOptype();
    double getPrice();
    int getCount();
    void setTime(time_t t);
    void setID(int id);
    void setCustomerID(int customerID);
    void setContractID(int contractID);
    void setContractName(std::string contractName);
    void setOptype(std::string optype);
    void setPrice(double price);
    void setCount(int count);
};
