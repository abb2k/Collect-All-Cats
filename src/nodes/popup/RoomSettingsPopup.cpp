#include "RoomSettingsPopup.hpp"
#include "../../utils/Save.hpp"
#include "../../layers/CatsLayer.hpp"

RoomSettingsPopup* RoomSettingsPopup::create() {
    auto ret = new RoomSettingsPopup();
    // @geode-ignore(unknown-resource)
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
    setCloseButtonSpr(CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"), .75f);

    auto menu = CCMenu::create();
    menu->setLayout(RowLayout::create()
        ->setGap(50)
        ->setCrossAxisAlignment(AxisAlignment::End)
        ->setGrowCrossAxis(true)
    );
    menu->setContentWidth(300);
    menu->setPosition({m_size.width / 2, 250});
    m_mainLayer->addChild(menu);

    auto BGSelectBtnSprite = CCSprite::createWithSpriteFrameName("bgIcon_01_001.png");
    BGSelectBtn = CCMenuItemSpriteExtra::create(
        BGSelectBtnSprite,
        nullptr,
        this,
        menu_selector(RoomSettingsPopup::openBGSelect)
    );
    menu->addChild(BGSelectBtn);

    auto GroundSelectBtnSprite = CCSprite::createWithSpriteFrameName("bgIcon_01_001.png");
    GroundSelectBtn = CCMenuItemSpriteExtra::create(
        GroundSelectBtnSprite,
        nullptr,
        this,
        menu_selector(RoomSettingsPopup::openGroundSelect)
    );
    menu->addChild(GroundSelectBtn);

    updateBGButton(Save::getBackground());
    updateGroundButton(Save::getGround());

    ccColor3B currBGColor = Save::getBackgroundColor();
    
    BGColorSelectBtnSprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    BGColorSelectBtnSprite->setColor(currBGColor);
    auto BGColorSelectBtn = CCMenuItemSpriteExtra::create(
        BGColorSelectBtnSprite,
        nullptr,
        this,
        menu_selector(RoomSettingsPopup::openBGColorSelect)
    );
    menu->addChild(BGColorSelectBtn);
    
    ccColor3B currGroundColor = Save::getGroundColor();

    GroundColorSelectBtnSprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    GroundColorSelectBtnSprite->setColor(currGroundColor);
    auto GroundColorSelectBtn = CCMenuItemSpriteExtra::create(
        GroundColorSelectBtnSprite,
        nullptr,
        this,
        menu_selector(RoomSettingsPopup::openGroundColorSelect)
    );
    menu->addChild(GroundColorSelectBtn);

    menu->updateLayout();

    return true;
}

void RoomSettingsPopup::selectArtClosed(SelectArtLayer* selectLater){
    if (selectLater == BGSelectLayer){

        Save::saveBackground(selectLater->m_art);
        CatsLayer::activeCatLayer()->bgLoader->setBackground(selectLater->m_art);
        updateBGButton(selectLater->m_art);

        BGSelectLayer = nullptr;
    }
    else if (selectLater == groundSelectLayer){

        Save::saveGround(selectLater->m_art);
        CatsLayer::activeCatLayer()->groundLoader->setGround(selectLater->m_art);
        updateGroundButton(selectLater->m_art);

        groundSelectLayer = nullptr;
    }
}


void RoomSettingsPopup::openBGSelect(CCObject*){
    int currBG = Save::getBackground();
    BGSelectLayer = SelectArtLayer::create(SelectArtType::Background, currBG);
    BGSelectLayer->m_delegate = this;
    BGSelectLayer->show();
}

void RoomSettingsPopup::openGroundSelect(CCObject*){
    int currGround = Save::getGround();
    groundSelectLayer = SelectArtLayer::create(SelectArtType::Ground, currGround);
    groundSelectLayer->m_delegate = this;
    groundSelectLayer->show();
    for (const auto& s : CCArrayExt<CCNode*>(groundSelectLayer->m_lineSprites))
        s->setVisible(false);
    static_cast<CCNode*>(groundSelectLayer->m_mainLayer->getChildren()->objectAtIndex(3))->setVisible(false);
}

void RoomSettingsPopup::updateBGButton(int BGID){
    CCSprite* toSet = nullptr;
    if (BGID < 10)
        toSet = CCSprite::createWithSpriteFrameName(fmt::format("bgIcon_0{}_001.png", BGID).c_str());
    else
        toSet = CCSprite::createWithSpriteFrameName(fmt::format("bgIcon_{}_001.png", BGID).c_str());
    BGSelectBtn->setSprite(toSet);
}

void RoomSettingsPopup::updateGroundButton(int GroundID){
    CCSprite* toSet = nullptr;
    if (GroundID < 10)
        toSet = CCSprite::createWithSpriteFrameName(fmt::format("gIcon_0{}_001.png", GroundID).c_str());
    else
        toSet = CCSprite::createWithSpriteFrameName(fmt::format("gIcon_{}_001.png", GroundID).c_str());
    GroundSelectBtn->setSprite(toSet);
}

void RoomSettingsPopup::updateColor(cocos2d::ccColor4B const& color){
    ccColor3B col3b = {color.r, color.g, color.b};
    if (lastClickedColorBtn == 1){
        BGColorSelectBtnSprite->setColor(col3b);
        CatsLayer::activeCatLayer()->bgLoader->setColor(col3b);
        Save::saveBackgroundColor(col3b);
    }
    else if (lastClickedColorBtn == 2){
        GroundColorSelectBtnSprite->setColor(col3b);
        CatsLayer::activeCatLayer()->groundLoader->setColor(col3b);
        Save::saveGroundColor(col3b);
    }
}

void RoomSettingsPopup::openBGColorSelect(CCObject*){
    ccColor3B currBGColor = Save::getBackgroundColor();
    auto colorPicker = ColorPickPopup::create(currBGColor);
    colorPicker->setDelegate(this);
    colorPicker->show();
    lastClickedColorBtn = 1;
}

void RoomSettingsPopup::openGroundColorSelect(CCObject*){
    ccColor3B currGroundColor = Save::getGroundColor();
    auto colorPicker = ColorPickPopup::create(currGroundColor);
    colorPicker->setDelegate(this);
    colorPicker->show();
    lastClickedColorBtn = 2;
}
