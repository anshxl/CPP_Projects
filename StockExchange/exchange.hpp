#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include "useraccount.hpp"
#include "utility.hpp"

class Exchange {
 public:
  void MakeDeposit(const std::string &username, const std::string &asset,
                   int amount);
  void PrintUserPortfolios(std::ostream &os) const;
  bool MakeWithdrawal(const std::string &username, const std::string &asset,
                      int amount);

  //Add Order and its helpers                    
  bool AddOrder(const Order &order);
  bool CheckOrder(const Order &order);
  void BuyOrder(Order& order);
  void ProcessBuy(Order& o1, Order& o2);
  void SellOrder(Order& order);
  void ProcessSell(Order& o1, Order& o2);
  bool CheckPrice(const Order& order, const Order& o2);

  //Print functions and helpers
  void PrintUsersOrders(std::ostream &os) const;
  std::string TradeRecord(const Trade&) const;
  void PrintTradeHistory(std::ostream &os) const;
  void PrintBidAskSpread(std::ostream &os) const;
  std::vector<std::string> SortUserNames() const;
  void ClearMarkets(std::vector<Order>&);
  Order CreateOrder(std::string name, std::string s, std::string a, int am, int pr);
  int FindBestPrice(const std::string&, const std::string&);
  std::vector<int> GetSortedIndices(const string&);
  //data members
  std::unordered_multimap<std::string, UserAccount> user_portfolios_;
  std::vector<Order> market;
  std::vector<Order> closed_orders;
  std::vector<std::string> trade_history;
  std::set<std::string> asset_names;
};