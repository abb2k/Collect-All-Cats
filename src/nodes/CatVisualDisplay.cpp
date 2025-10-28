#include "CatVisualDisplay.hpp"

CatVisualDisplay* CatVisualDisplay::create() {
    auto ret = new CatVisualDisplay();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CatVisualDisplay::init() {
    if (!CCMenu::init()) return false;

    this->setPosition({0, 0});
    this->setContentSize({150, 150});
    this->ignoreAnchorPointForPosition(false);

    return true;
}

void CatVisualDisplay::updateVisuals(CatStats& stats){
    
    if (!current.has_value() || stats.catTypeID != current.value().catTypeID)
        this->updateCoreSprites(stats.catTypeID);

    if (!current.has_value() || stats.primaryColor != current.value().primaryColor)
        this->updateCorePrimary(stats.primaryColor);
    
    if (!current.has_value() || stats.secondaryColor != current.value().secondaryColor)
        this->updateCoreSecondary(stats.secondaryColor);

    current = stats;
}

void CatVisualDisplay::updateCoreSprites(unsigned int typeID){
    if (auto primary = this->getChildByID("kitty-color-three-sprite-primary")) {
        this->removeChild(primary, true);
    }
    if (auto secondary = this->getChildByID("kitty-color-three-sprite-secondary")) {
        this->removeChild(secondary, true);
    }

    auto sprites = CatStats::getSpritesPathsForCat(typeID);

    auto KCTSPrimary = CCSprite::create(sprites.first.c_str());
    KCTSPrimary->setID("kitty-color-three-sprite-primary");
    KCTSPrimary->setScale(.7f);
    KCTSPrimary->setPosition(this->getContentSize() / 2);
    this->addChild(KCTSPrimary);

    auto KCTSSecondary = CCSprite::create(sprites.second.c_str());
    KCTSSecondary->setID("kitty-color-three-sprite-secondary");
    KCTSSecondary->setScale(.7f);
    KCTSSecondary->setPosition(this->getContentSize() / 2);
    this->addChild(KCTSSecondary);
}

void CatVisualDisplay::updateCorePrimary(ccColor4B primary){
    auto spriteNode = this->getChildByID("kitty-color-three-sprite-primary");
    if (auto spr = typeinfo_cast<CCSprite*>(spriteNode)) {
        spr->setColor({primary.r, primary.g, primary.b});
        spr->setOpacity(primary.a);
    }
}
void CatVisualDisplay::updateCoreSecondary(ccColor4B secondary){
    auto spriteNode = this->getChildByID("kitty-color-three-sprite-secondary");
    if (auto spr = typeinfo_cast<CCSprite*>(spriteNode)) {
        spr->setColor({secondary.r, secondary.g, secondary.b});
        spr->setOpacity(secondary.a);
    }
}