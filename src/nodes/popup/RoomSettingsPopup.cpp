#include "RoomSettingsPopup.hpp"

RoomSettingsPopup* RoomSettingsPopup::create() {
    auto ret = new RoomSettingsPopup();
    if (ret->initAnchored(400, 300, "geode.loader/GE_square03.png")) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool RoomSettingsPopup::setup() {
    
    m_bgSprite->setOpacity(220);
    m_closeBtn->setPosition({m_closeBtn->getPositionX() + m_closeBtn->getContentWidth() / 2, m_closeBtn->getPositionY() - m_closeBtn->getContentHeight() / 2});
    auto backSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    backSprite->setScale(.75f);
    m_closeBtn->setSprite(backSprite);

    return true;
}
