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

    this->scheduleUpdate();

    return true;
}

void LinkedCatDisplay::setCat(const CatStats& catStats){
    float scale = CCDirector::sharedDirector()->getContentScaleFactor();

    kittyColonThreeSprite = CCRenderTexture::create(150 * scaleFactor, 150 * scaleFactor);
    kittyColonThreeSprite->setScale(kittyColonThreeSprite->getSprite()->getContentWidth() / 66.66f / scaleFactor);
    kittyColonThreeSprite->setPosition(kittyColonThreeSprite->getSprite()->getContentSize() / 2);
    this->addChild(kittyColonThreeSprite);

    this->setContentSize(kittyColonThreeSprite->getSprite()->getContentSize());
    this->setAnchorPoint({.5f, .5f});

    catDisplay = CatVisualDisplay::create();
    catDisplay->setVisible(false);
    catDisplay->setScale(this->getContentWidth() / 66.66f);
    catDisplay->setPosition(this->getContentSize() / 2);
    this->addChild(catDisplay);

    auto shadow = CCSprite::createWithSpriteFrameName("d_circle_02_001.png");
    shadow->setZOrder(-1);
    shadow->setScale(kittyColonThreeSprite->getSprite()->getContentWidth() / shadow->getContentWidth());
    shadow->setScaleY(shadow->getScaleY() / 2);
    shadow->setPosition(kittyColonThreeSprite->getPosition() - ccp(0, kittyColonThreeSprite->getSprite()->getContentHeight() / 2));
    shadow->setColor({0,0,0});
    shadow->setOpacity(150);
    this->addChild(shadow);

    nameLabel = CCLabelBMFont::create(catStats.name.c_str(), "goldFont.fnt");
    nameLabel->setPosition(kittyColonThreeSprite->getSprite()->getContentSize() / 2 + ccp(0, kittyColonThreeSprite->getSprite()->getContentHeight() / 1.5f));
    nameLabel->setVisible(false);
    this->addChild(nameLabel);

    myStats = Ok(catStats);
}

void LinkedCatDisplay::update(float dt){
    nameLabel->setScale(std::clamp(150 / nameLabel->getContentWidth(), .4f, 1.0f));
    
    if (myStats.isErr()) return;

    auto catsLayer = CatsLayer::activeCatLayer();
    if (!catsLayer) return;

    auto stats = myStats.unwrap();
    
    auto catRef = catsLayer->getCatFromStats(stats);
    if (catRef == nullptr){
        kittyColonThreeSprite->beginWithClear(0, 0, 0, 0);
        kittyColonThreeSprite->end();
        catDisplay->setVisible(true);
        catDisplay->updateVisuals(stats);
        return;
    }
    catDisplay->setVisible(false);

    float scale = CCDirector::sharedDirector()->getContentScaleFactor();

    auto visual = catRef->getVisualParent();
    auto oldPos = visual->getPosition();
    auto oldSize = visual->getScale();
    auto textSize = kittyColonThreeSprite->getSprite()->getContentSize();

    visual->setPosition({(textSize.width - visual->getContentSize().width) / 2, (textSize.height - visual->getContentSize().height) / 2});
    visual->setScale(scaleFactor);
    
    kittyColonThreeSprite->beginWithClear(0, 0, 0, 0);
    visual->visit();
    kittyColonThreeSprite->end();
    
    visual->setScale(oldSize);
    visual->setPosition(oldPos);
}

void LinkedCatDisplay::setNameAboveVisible(bool b){
    nameLabel->setVisible(b);
}