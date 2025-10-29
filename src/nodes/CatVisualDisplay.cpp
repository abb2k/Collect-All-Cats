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

void CatVisualDisplay::updateVisuals(const CatStats& stats){
    const CatStats* cur = current.has_value() ? &current.value() : nullptr;

    if (!cur || stats.catTypeID != cur->catTypeID)
        this->updateCoreSprites(stats.catTypeID);

    if (!cur || stats.primaryColor != cur->primaryColor)
        this->updateCorePrimary(stats.primaryColor);

    if (!cur || stats.secondaryColor != cur->secondaryColor)
        this->updateCoreSecondary(stats.secondaryColor);

    current = stats;
}

void CatVisualDisplay::initSprite(CCSprite* spr, const char* id) {
    if (!spr) return;
    spr->setID(id);
    spr->setScale(.7f);
    spr->setPosition(this->getContentSize() / 2);
    this->addChild(spr);
}

void CatVisualDisplay::updateCoreSprites(unsigned int typeID){
    if (m_primarySprite) {
        this->removeChild(m_primarySprite, true);
        m_primarySprite = nullptr;
    }
    if (m_secondarySprite) {
        this->removeChild(m_secondarySprite, true);
        m_secondarySprite = nullptr;
    }
    if (m_noncolorSprite) {
        this->removeChild(m_noncolorSprite, true);
        m_noncolorSprite = nullptr;
    }

    std::thread([this, typeID]() {
        geode::queueInMainThread([this, typeID]() {
            auto sprites = CatStats::createSpritesPathsForCat(typeID);

            if (sprites.primary) {
                m_primarySprite = sprites.primary;
                initSprite(m_primarySprite, "kitty-color-three-sprite-primary");
            }

            if (sprites.secondary.has_value()){
                m_secondarySprite = sprites.secondary.value();
                initSprite(m_secondarySprite, "kitty-color-three-sprite-secondary");
            }

            if (sprites.noncolor.has_value()){
                m_noncolorSprite = sprites.noncolor.value();
                initSprite(m_noncolorSprite, "kitty-color-three-sprite-noncolor");
            }
        });

    }).detach();
}

void CatVisualDisplay::updateCorePrimary(ccColor4B primary){
    if (m_primarySprite == nullptr) return;

    m_primarySprite->setColor({primary.r, primary.g, primary.b});
    m_primarySprite->setOpacity(primary.a);
}

void CatVisualDisplay::updateCoreSecondary(ccColor4B secondary){
    if (m_secondarySprite  == nullptr) return;

    m_secondarySprite->setColor({secondary.r, secondary.g, secondary.b});
    m_secondarySprite->setOpacity(secondary.a);
}