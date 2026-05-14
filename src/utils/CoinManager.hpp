#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MoneyChangedEvent : public Event<MoneyChangedEvent, bool(int newCoinAmount)> {
public:
    using Event::Event;
};
class BuyItemEvent : public Event<BuyItemEvent, bool(std::string itemCategory, std::string itemID)> {
public:
    using Event::Event;
};
class SellItemEvent : public Event<SellItemEvent, bool(std::string itemCategory, std::string itemID)> {
public:
    using Event::Event;
};

class CoinManager{
    public:
        static int getCoinCount();

        static void buyItem(const std::string& itemCategory, const std::string& itemID);
        static void sellItem(const std::string& itemCategory, const std::string& itemID);

        static int COINS_PER_LVL;
};