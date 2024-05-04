#include"utils.h"
std::string serializeMap(const std::map<std::string, std::string>& data)
{
    std::string result;
    for (auto it = data.begin(); it != data.end(); it++) {
        result += it->first + "=" + it->second + "&";
    }
    return result;
}
std::map<std::string, std::string> deserializeMap(const std::string& str)
{
    std::map<std::string, std::string> result;
    std::string key;
    std::string value;
    bool isKey = true;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '=') {
            isKey = false;
            continue;
        }
        if (str[i] == '&') {
            result[key] = value;
            key = "";
            value = "";
            isKey = true;
            continue;
        }
        if (isKey) {
            key += str[i];
        }
        else {
            value += str[i];
        }
    }
    return result;
}

WeiTuo str_to_WeiTuo(std::string str)
{
    std::map<std::string, std::string> w = deserializeMap(str);
    WeiTuo wt;
    wt.setCustomerID(std::stoi(w["customerID"]));
    wt.setContractID(std::stoi(w["contractID"]));
    wt.setContractName(w["contractName"]);
    wt.setOptype(w["optype"]);
    wt.setPrice(std::stod(w["price"]));
    wt.setCount(std::stoi(w["count"]));
    wt.setID(-1);
    return wt;
}
std::string Weituo_to_str(WeiTuo wt)
{
    std::map<std::string, std::string> w;
    w["op"]="weituo";
    w["customerID"] = std::to_string(wt.getCustomerID());
    w["contractID"] = std::to_string(wt.getContractID());
    w["contractName"] = wt.getContractName();
    w["optype"] = wt.getOptype();
    w["price"] = std::to_string(wt.getPrice());
    w["count"] = std::to_string(wt.getCount());
    return serializeMap(w);
}
std::string login_info(std::string customerID,std::string password)
{
    std::map<std::string,std::string> l;
    l["op"]="login";
    l["customerID"]=customerID;
    l["password"]=password;
    return serializeMap(l);
}
