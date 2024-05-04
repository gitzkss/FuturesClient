#pragma once
#include <string>
#include <stack>
class ChiCang
{
private:
    int id;
    int contractID;
    std::string contractName;
    double duoSum;
    double kongSum;
    std::stack<double>duo;
    std::stack<double>kong;
public:
    ChiCang(int id = -1, int contractID = -1, std::string contractName="");
    ~ChiCang();
    double getAvgDuo();
    double getAvgKong();
    void addDuo(double price);
    void addKong(double price);
    void removeDuo(int count);
    void removeKong(int count);
    int getDuoCount();
    int getKongCount();
};
