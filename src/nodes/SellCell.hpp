#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SellCell : public CCMenu {
    public:
        static SellCell* create(const std::string& itemCategory, const std::string& itemID);
        
    private:
        bool init(const std::string& itemCategory, const std::string& itemID);
};