#include "BankruptPopup.hpp"

#include <layers/CatsLayer.hpp>

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