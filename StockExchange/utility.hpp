#pragma once
#include <iostream>
#include <string>
#include <sstream>

struct Order {
  std::string username;
  std::string side;  // Can be "Buy" or "Sell"
  std::string asset;
  int amount;
  int price;
  Order(std::string u, std::string s, std::string a, int am, int pr): username(u), 
  side(s), asset(a), amount(am), price(pr) {};
  Order() = default;
  friend std::ostream& operator<<(std::ostream& os,Order const& order){
    os << order.side << " " << order.amount << " " << order.asset << " at " << order.price << " USD by " << order.username;
    return os;
  }
  friend bool operator==(Order const& o1, Order const& o2){
    int count = 0;
    if(o1.username == o2.username) ++count;
    if(o1.side == o2.side) ++count;
    if(o1.asset == o2.asset) ++count;
    if(o1.amount == o2.amount) ++count;
    if(o1.price == o2.price) ++count;

    if(count == 5) return true;
    return false;
  }
};

struct Trade {
  std::string buyer_username;
  std::string seller_username;
  std::string asset;
  int amount;
  int price;
  Trade(std::string b, std::string s, std::string a, int am, int pr){
    buyer_username = b;
    seller_username = s;
    asset = a;
    amount = am;
    price = pr;
  }
};