#include "chi_cang.h"

ChiCang::ChiCang(int id, int contractID, std::string contractName) :id(id), contractID(contractID), contractName(contractName) {};
ChiCang::~ChiCang() {};
void ChiCang::addDuo(double price)
{
    this->duo.push(price);
    this->duoSum += price;
}
void ChiCang::addKong(double price)
{
    this->kong.push(price);
    this->kongSum += price;
}
void ChiCang::removeDuo(int count)
{
    if (count > duo.size())
    {
        //error
        return;
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            duoSum -= duo.top();
            duo.pop();
        }
    }
}
void ChiCang::removeKong(int count)
{
    if (count > kong.size())
    {
        //error
        return;
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            kongSum -= kong.top();
            kong.pop();
        }
    }
}
double ChiCang::getAvgDuo()
{
    if (duo.size() == 0)
    {
        return 0;
    }
    else
    {
        return duoSum / duo.size();
    }
}
double ChiCang::getAvgKong()
{
    if (kong.size() == 0)
    {
        return 0;
    }
    else
    {
        return kongSum / kong.size();
    }
}
int ChiCang::getDuoCount()
{
    return duo.size();
}
int ChiCang::getKongCount()
{
    return kong.size();
}
