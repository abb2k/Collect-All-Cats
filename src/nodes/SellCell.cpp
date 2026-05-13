#include "SellCell.hpp"

SellCell* SellCell::create(const std::string& itemCategory, const std::string& itemID){
    auto ret = new SellCell();
    if (ret && ret->init(itemCategory, itemID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SellCell::init(const std::string& itemCategory, const std::string& itemID){
    if (!CCMenu::init()) return false;

    

    return true;
}
