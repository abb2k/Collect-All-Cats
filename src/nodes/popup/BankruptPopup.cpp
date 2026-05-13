#include "BankruptPopup.hpp"

#include <layers/CatsLayer.hpp>
#include <nodes/SellCell.hpp>
#include <utils/Save.hpp>

BankruptPopup* BankruptPopup::create() {
    auto ret = new BankruptPopup();
    // @geode-ignore(unknown-resource)
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BankruptPopup::init() {
    // @geode-ignore(unknown-resource)
    if (!Popup::init(400, 250, "geode.loader/GE_square01.png")) return false;

    auto catsLayer = CatsLayer::activeCatLayer();
    if (!catsLayer) return false;

    ScrollLayer* scrollLayer = ScrollLayer::create(m_size / 1.2f - ccp(0, 20));
    m_mainLayer->addChild(scrollLayer);

    auto accessO = Save::getUnlockedAccessories();

    for (const auto& entry : accessO) {

        auto splitStr = utils::string::split(entry, "-");
        if (splitStr.size() != 2) continue;

        auto sellCell = SellCell::create(splitStr[0], splitStr[1]);
        if (sellCell == nullptr) continue;

        scrollLayer->m_contentLayer->addChild(sellCell);
    }

    return true;
}

void BankruptPopup::show(){
    Popup::show();
}

void BankruptPopup::onClose(CCObject*){
    Popup::onClose(nullptr);
}

void BankruptPopup::onEnter(){
    Popup::onEnter();
}

void BankruptPopup::onExit(){
    Popup::onExit();
}