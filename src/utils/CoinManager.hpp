#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CoinManager{
    public:
        static int getCoinCount();

        static void buyItem(const std::string& itemCategory, const std::string& itemID);
        static void sellItem(const std::string& itemCategory, const std::string& itemID);

        static int COINS_PER_LVL;
};