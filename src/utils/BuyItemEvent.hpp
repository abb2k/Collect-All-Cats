#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BuyItemEvent : public Event<BuyItemEvent, bool(int newCoinAmount)> {
public:
    using Event::Event;
};