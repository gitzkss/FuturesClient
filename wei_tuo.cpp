#include"wei_tuo.h"

WeiTuo::WeiTuo(time_t t, int id, int customerID, int contractID, std::string contractName, std::string optype, double price, int count)
    :time(t), id(id), customerID(customerID), contractID(contractID), contractName(contractName), optype(optype), price(price), count(count)
{
    if (time == -1)
        this->time = ::time(nullptr);

}


WeiTuo::~WeiTuo() {};
time_t WeiTuo::getTime()
{
    return time;
}
int WeiTuo::getID()
{
    return id;
}
int WeiTuo::getCustomerID()
{
    return customerID;
}
int WeiTuo::getContractID()
{
    return contractID;
}
std::string WeiTuo::getContractName()
{
    return contractName;
}
std::string WeiTuo::getOptype()
{
    return optype;
}
double WeiTuo::getPrice()
{
    return price;
}
int WeiTuo::getCount()
{
    return count;
}
void WeiTuo::setTime(time_t t)
{
    time = t;
}
void WeiTuo::setID(int id)
{
    this->id = id;
}
void WeiTuo::setCustomerID(int id)
{
    customerID = id;
}
void WeiTuo::setContractID(int id)
{
    contractID = id;
}
void WeiTuo::setContractName(std::string name)
{
    contractName = name;
}
void WeiTuo::setOptype(std::string type)
{
    optype = type;
}
void WeiTuo::setPrice(double price)
{
    this->price = price;
}
void WeiTuo::setCount(int count)
{
    this->count = count;
}

