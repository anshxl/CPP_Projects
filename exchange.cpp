#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <unordered_map>
using std::unordered_map;
#include <algorithm>
#include <numeric>
#include <iterator>
#include <set>
using std::set;
#include "useraccount.hpp"
#include "utility.hpp"
#include "exchange.hpp"

void Exchange::MakeDeposit(const string& username, const string& asset,
                           int amount) {
  auto it = user_portfolios_.find(username);
  if (it == user_portfolios_.end()) {
    user_portfolios_.insert(std::make_pair(username, UserAccount()));
    it = user_portfolios_.find(username);
  }
  it->second.AddAssets(asset, amount);
  asset_names.insert(asset);
}

vector<string> Exchange::SortUserNames() const {
  vector<string> user_names;
  for (const auto& pair : user_portfolios_) {
    user_names.push_back(pair.first);
  }
  std::sort(user_names.begin(), user_names.end());
  return user_names;
}

void Exchange::PrintUserPortfolios(std::ostream& os) const {
  // store user names
  os << "User Portfolios (in alphabetical order):" << endl;
  vector<string> user_names = SortUserNames();
  for (const auto& user : user_names) {
    os << user << "'s Portfolio: ";
    // find the asset map associated to each user
    for (const auto& account : user_portfolios_) {
      if (account.first == user) {
        const auto& assets = account.second.GetAssets();
        vector<string> asset_name;
        // iterate through the map to get the asset name
        for (const auto& pair : assets) {
          asset_name.push_back(pair.first);
        }
        // sort the asset names
        std::sort(asset_name.begin(), asset_name.end());
        for (const string& name : asset_name) {
          if (assets.at(name) != 0)
            os << assets.at(name) << " " << name << ", ";
        }
      }
    }
    os << endl;
  }
}

bool Exchange::MakeWithdrawal(const string& username, const string& asset,
                              int amount) {
  auto it = user_portfolios_.find(username);
  if (it == user_portfolios_.end()) {
    cout << "User does not exist" << endl;
    return false;
  } else if (it->second.CheckAsset(asset, amount) == 0) {
    std::cerr << "Insufficient Funds for Withdrawal." << endl;
    return false;
  } else {
    it->second.WithdrawAssets(asset, amount);
    return true;
  }
}

Order Exchange::CreateOrder(std::string name, std::string s, std::string a,
                            int am, int pr) {
  Order o;
  o.username = name;
  o.side = s;
  o.asset = a;
  o.amount = am;
  o.price = pr;
  return o;
}

bool Exchange::CheckOrder(const Order& order) {
  const string currency = "USD";
  auto it = user_portfolios_.find(order.username);
  if (order.side == "Buy") {
    if (it->second.CheckAsset(currency, (order.amount) * (order.price)) == 0) {
      std::cerr << "Can't place order because " << order.username
                << " does not have enough USD." << endl;
      return false;
    }
  } else {
    if (it->second.CheckAsset(order.asset, order.amount) == 0) {
      return false;
    }
  }
  return true;
}

bool Exchange::CheckPrice(const Order& o1, const Order& o2) {
  if (o1.side == "Buy" && o2.side == "Sell") {
    return o1.price >= o2.price;
  } else if (o1.side == "Sell" && o2.side == "Buy") {
    return o1.price <= o2.price;
  }
  return false;
}

//This function sorts the market according the best prices
//If the taker orer is a buy, it sorts the market to have the 
//lowest sell price at the front. Conversely, if the taker
//order is a buy, it keeps the highest buy price at the front
vector<int> Exchange::GetSortedIndices(const string& side) {
  vector<int> indices(market.size());
  std::iota(indices.begin(), indices.end(), 0);
  if (side == "Sell") {
    std::stable_sort(indices.begin(), indices.end(), [&](size_t i, size_t j) {
      return market[i].price > market[j].price;
    });
  } else {
    std::stable_sort(indices.begin(), indices.end(), [&](size_t i, size_t j) {
      return market[i].price < market[j].price;
    });
  }
  return indices;
}

//Remove filled orders from the market
void Exchange::ClearMarkets(vector<Order>& m) {
  m.erase(std::remove_if(m.begin(), m.end(), [](Order& x) {
            return x.amount == 0;
          }), m.end());
}

//Handle the market changes associated with a Buy Order
void Exchange::ProcessBuy(Order& order, Order& sellers) {
  if (order.amount >= sellers.amount) {
    closed_orders.push_back(CreateOrder(order.username, order.side, order.asset,
                                        sellers.amount, order.price));
    order.amount -= sellers.amount;
    sellers.price = order.price;
    closed_orders.push_back(sellers);
    sellers.amount = 0;
  } else if (order.amount < sellers.amount) {
    closed_orders.push_back(CreateOrder(sellers.username, sellers.side,
                                        sellers.asset, order.amount,
                                        order.price));
    sellers.amount -= order.amount;
    closed_orders.push_back(order);
    order.amount = 0;
  }
}

//Handle the transactions associated with a Buy Order
void Exchange::BuyOrder(Order& order) {
  vector<int> sorted_market = GetSortedIndices("Buy");
  for (const auto& idx : sorted_market) {
    auto& sellers = market[idx];
    if (sellers.asset == order.asset && sellers.side == "Sell" &&
        CheckPrice(order, sellers) == 1) {
      MakeDeposit(order.username, order.asset,
                  std::min(sellers.amount, order.amount));
      MakeDeposit(sellers.username, "USD",
                  std::min(sellers.amount, order.amount) * order.price);
      trade_history.push_back(TradeRecord(
          Trade(order.username, sellers.username, order.asset,
                std::min(sellers.amount, order.amount), order.price)));
      if (order.amount >= sellers.amount) {
        ProcessBuy(order, sellers);
      } else if (order.amount < sellers.amount) {
        ProcessBuy(order, sellers);
        break;
      } else
        continue;
    }
  }
  if (order.amount != 0) {
    market.push_back(order);
  }
  ClearMarkets(market);
}

//Handle the market changes associated with a sell order
void Exchange::ProcessSell(Order& order, Order& buyers) {
  if (order.amount >= buyers.amount) {
    order.amount -= buyers.amount;
    buyers.price = order.price;
    closed_orders.push_back(buyers);
    closed_orders.push_back(CreateOrder(order.username, order.side, order.asset,
                                        buyers.amount, order.price));
    buyers.amount = 0;
  } else if (order.amount < buyers.amount) {
    closed_orders.push_back(CreateOrder(
        buyers.username, buyers.side, buyers.asset, order.amount, order.price));
    buyers.amount -= order.amount;
    closed_orders.push_back(order);
    order.amount = 0;
  }
}

//Handle the transactions associated with a sell order
void Exchange::SellOrder(Order& order) {
  vector<int> sorted_market = GetSortedIndices("Sell");
  for (const auto& idx : sorted_market) {
    auto& buyers = market[idx];
    if (buyers.asset == order.asset && buyers.side == "Buy" &&
        CheckPrice(order, buyers) == 1) {
      MakeDeposit(order.username, "USD",
                  std::min(order.amount, buyers.amount) * order.price);
      MakeDeposit(buyers.username, buyers.asset,
                  std::min(buyers.amount, order.amount));
      trade_history.push_back(TradeRecord(
          Trade(buyers.username, order.username, order.asset,
                std::min(order.amount, buyers.amount), order.price)));
      if (order.amount >= buyers.amount) {
        ProcessSell(order, buyers);
      } else if (order.amount < buyers.amount) {
        ProcessSell(order, buyers);
        break;
      } else
        continue;
    }
  }
  if (order.amount != 0) {
    market.push_back(order);
  }
  ClearMarkets(market);
}

//Combine all Buy/Sell Helper functions
bool Exchange::AddOrder(const Order& order) {
  if (CheckOrder(order) == 0)
    return false;
  else {
    Order nc_order = order;
    if (order.side == "Buy") {
      MakeWithdrawal(order.username, "USD", order.amount * order.price);
      BuyOrder(nc_order);
      return true;
    } else {
      MakeWithdrawal(order.username, order.asset, order.amount);
      SellOrder(nc_order);
      return true;
    }
  }
}

void Exchange::PrintUsersOrders(std::ostream& os) const {
  os << "Users Orders (in alphabetical order):" << endl;
  vector<string> user_names = SortUserNames();
  for (const auto& name : user_names) {
    os << name << "'s Open Orders (in chronological order):" << endl;
    for (const auto& order : market) {
      if (order.username == name) {
        os << order << endl;
      }
    }
    os << name << "'s Filled Orders (in chronological order):" << endl;
    for (const auto& order : closed_orders) {
      if (order.username == name) {
        os << order << endl;
      }
    }
  }
}

string Exchange::TradeRecord(const Trade& trade) const {
  std::ostringstream oss;
  vector<string> record;
  oss << trade.buyer_username << " Bought " << trade.amount << " of "
      << trade.asset << " From " << trade.seller_username << " for "
      << trade.price << " USD";
  return oss.str();
}

void Exchange::PrintTradeHistory(std::ostream& os) const {
  os << "Trade History (in chronological order):" << endl;
  for (const auto& record : trade_history) {
    os << record << endl;
  }
}

void Exchange::PrintBidAskSpread(std::ostream& os) const {
  os << "Asset Bid Ask Spread (in alphabetical order):" << endl;
  for (const auto& elem : asset_names) {
    if (elem == "USD") {
      cout << endl;
      break;
    }
    int max_buy_price = 0;
    int min_sell_price = 10000;
    os << elem << ": ";
    for (const auto& order : market) {
      if (order.asset == elem && order.side == "Buy") {
        max_buy_price = std::max(order.price, max_buy_price);
      } else if (order.asset == elem && order.side == "Sell") {
        min_sell_price = std::min(order.price, min_sell_price);
      }
    }
    if (min_sell_price != 10000 && max_buy_price != 0) {
      os << "Highest Open Buy = " << max_buy_price
         << " USD and Lowest Open Sell = " << min_sell_price << " USD" << endl;
    } else if (min_sell_price != 10000 && max_buy_price == 0) {
      os << "Highest Open Buy = NA USD and Lowest Open Sell = "
         << min_sell_price << " USD" << endl;
    } else {
      os << "Highest Open Buy = " << max_buy_price
         << " USD and Lowest Open Sell = NA USD" << endl;
    }
  }
}

int main(){
    Exchange e;
  std::ostringstream oss;
  e.MakeDeposit("Nahum", "BTC", 1000);
  e.MakeDeposit("Nahum", "USD", 100000);
  e.MakeDeposit("Dolson", "USD", 555555);
  e.MakeDeposit("Ofria", "ETH", 678);
  e.MakeDeposit("Zaabar", "USD", 12121212);
  e.MakeDeposit("Zaabar", "LTC", 4563);
  // BTC rough price 1000
  // ETH rough price 100
  // LTC rough price 500
  e.AddOrder({"Nahum", "Sell", "BTC", 5, 1100});
  e.AddOrder({"Nahum", "Sell", "BTC", 100, 1200});
  e.AddOrder({"Nahum", "Buy", "BTC", 7, 800});
  e.AddOrder({"Dolson", "Buy", "BTC", 1, 950});
  e.AddOrder({"Ofria", "Sell", "ETH", 12, 156});
  e.AddOrder({"Ofria", "Sell", "ETH", 10, 160});
  e.AddOrder({"Zaabar", "Sell", "LTC", 10, 550});
  e.AddOrder({"Zaabar", "Buy", "LTC", 10, 450});
  e.AddOrder({"Nahum", "Buy", "LTC", 55, 600});
  e.AddOrder({"Nahum", "Buy", "ETH", 30, 158});
  e.AddOrder({"Ofria", "Sell", "ETH", 10, 140});
  e.AddOrder({"Zaabar", "Buy", "BTC", 10, 1500});
  e.AddOrder({"Zaabar", "Buy", "ETH", 200, 1200});
  e.AddOrder({"Nahum", "Sell", "BTC", 30, 158});
  e.AddOrder({"Dolson", "Buy", "BTC", 20, 2000});
  e.AddOrder({"Nahum", "Sell", "ETH", 1, 1423});
  e.PrintUsersOrders(std::cout);
}