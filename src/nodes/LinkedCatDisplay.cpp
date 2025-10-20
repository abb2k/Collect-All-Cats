#include "LinkedCatDisplay.hpp"

#include <layers/CatsLayer.hpp>

LinkedCatDisplay* LinkedCatDisplay::create(CatStats* catStats) {
    auto ret = new LinkedCatDisplay();
    if (ret->init(catStats)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool LinkedCatDisplay::init(CatStats* catStats) {
    if (!CCMenu::init()) return false;

    this->setPosition({0, 0});

    if (catStats != nullptr)
        setCat(*catStats);

    ignoreAnchorPointForPosition(false);

    return true;
}

void LinkedCatDisplay::setCat(const CatStats& catStats){
    auto catsLayer = CatsLayer::activeCatLayer();
    if (!catsLayer) return;

    auto kittyColonThreeSprite = CCSprite::createWithSpriteFrameName("default_cat.png"_spr);
    kittyColonThreeSprite->setPosition(kittyColonThreeSprite->getContentSize() / 2);
    kittyColonThreeSprite->setScale(1.5f);
    this->addChild(kittyColonThreeSprite);

    this->setContentSize(kittyColonThreeSprite->getContentSize());
    this->setAnchorPoint({.5f, .5f});

    auto shadow = CCSprite::createWithSpriteFrameName("d_circle_02_001.png");
    shadow->setZOrder(-1);
    shadow->setScale(kittyColonThreeSprite->getContentWidth() / shadow->getContentWidth());
    shadow->setScaleY(shadow->getScaleY() / 2);
    shadow->setPosition(kittyColonThreeSprite->getPosition() - ccp(0, kittyColonThreeSprite->getContentHeight() / 2));
    shadow->setColor({0,0,0});
    shadow->setOpacity(150);
    this->addChild(shadow);
}