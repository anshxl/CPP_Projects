
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

class UserAccount{
    public:
      //std::string username;
      std::unordered_map<std::string, int> user_assets_;

      //constructors
      UserAccount() = default;
      
      //member functions
      void AddAssets(const std::string& asset, int amount){
      user_assets_[asset] += amount;
      }

      void WithdrawAssets(const std::string& asset, int amount){
        if(!CheckAsset(asset, amount)){
        std::cerr << "Insufficient Funds for " << asset << std::endl;
        }
        user_assets_[asset] -= amount;
      }

      bool CheckAsset(const std::string& asset, int amount){
        auto it = user_assets_.find(asset);
        return (it != user_assets_.end() && it->second >= amount);
      }

      std::unordered_map<std::string, int> GetAssets() const {
        return user_assets_;
      }

      friend class Exchange;
};
