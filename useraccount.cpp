#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

#include "useraccount.hpp"

int main(){
    UserAccount my_portfolio;
    my_portfolio.AddAssets("BTC", 100);
    my_portfolio.AddAssets("USD", 1000);
    for(const auto& pair: my_portfolio.user_assets_){
        std::cout << pair.first << ": " << pair.second << std::endl;
    }   
    std::cout << my_portfolio.CheckAsset("USD", 1000) << std::endl;
    std::cout << my_portfolio.CheckAsset("ETH", 200) << std::endl;
}