#pragma once
#include<map>
#include<string>
#include<map>
#include"wei_tuo.h"
#include<iostream>
#define bug(x) (std::cout<<"*** "<<x<<std::endl)
#define bug2(x,y) (std::cout<<"*** "<<x<<" "<<y<<std::endl)
#define bug3(x,y,z) (std::cout<<"*** "<<x<<" "<<y<<" "<<z<<std::endl)
#define bug4(x,y,z,w) (std::cout<<"*** "<<x<<" "<<y<<" "<<z<<" "<<w<<std::endl)
#define bug5(x,y,z,w,v) (std::cout<<"*** "<<x<<" "<<y<<" "<<z<<" "<<w<<" "<<v<<std::endl)
std::map<std::string, std::string> deserializeMap(const std::string& str);
std::string serializeMap(const std::map<std::string, std::string>& map);
WeiTuo str_to_WeiTuo(std::string str);
std::string Weituo_to_str(WeiTuo wt);
std::string login_info(std::string customerID,std::string password);
