#include "RoomSettingsPopup.hpp"

#include <utils/Save.hpp>
#include <layers/CatsLayer.hpp>

RoomSettingsPopup* RoomSettingsPopup::create() {
    auto ret = new RoomSettingsPopup();
    // @geode-ignore(unknown-resource)
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool RoomSettingsPopup::init() {
    // @geode-ignore(unknown-resource)
    if (!Popup::init(400, 300, "geode.loader/GE_square03.png")) return false;
    
    m_bgSprite->setOpacity(220);
    m_closeBtn->setPosition({m_closeBtn->getPositionX() + m_closeBtn->getContentWidth() / 2, m_closeBtn->getPositionY() - m_closeBtn->getContentHeight() / 2});
    setCloseButtonSpr(CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"), .75f);

    setTitle("Room Options");

    auto menuBG = CCMenu::create();
    menuBG->setLayout(RowLayout::create()
        ->setGap(50)
        ->setCrossAxisAlignment(AxisAlignment::End)
        ->setGrowCrossAxis(true)
    );
    menuBG->setContentWidth(120);
    menuBG->setPosition({35, 210});
    menuBG->setAnchorPoint({0, 0.5f});
    menuBG->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChild(menuBG);

    auto menuG = CCMenu::create();
    menuG->setLayout(RowLayout::create()
        ->setGap(50)
        ->setCrossAxisAlignment(AxisAlignment::End)
        ->setGrowCrossAxis(true)
    );
    menuG->setContentWidth(120);
    menuG->setPosition({m_size.width - 35, 210});
    menuG->setAnchorPoint({1, 0.5f});
    menuG->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChild(menuG);

    auto BGSelectBtnSprite = CCSprite::createWithSpriteFrameName("bgIcon_01_001.png");
    BGSelectBtn = CCMenuItemSpriteExtra::create(
        BGSelectBtnSprite,
        nullptr,
        this,
        menu_selector(RoomSettingsPopup::openBGSelect)
    );
    menuBG->addChild(BGSelectBtn);

    auto GroundSelectBtnSprite = CCSprite::createWithSpriteFrameName("bgIcon_01_001.png");
    GroundSelectBtn = CCMenuItemSpriteExtra::create(
        GroundSelectBtnSprite,
        nullptr,
        this,
        menu_selector(RoomSettingsPopup::openGroundSelect)
    );
    menuG->addChild(GroundSelectBtn);

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
    menuBG->addChild(BGColorSelectBtn);
    
    ccColor3B currGroundColor = Save::getGroundColor();

    GroundColorSelectBtnSprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    GroundColorSelectBtnSprite->setColor(currGroundColor);
    auto GroundColorSelectBtn = CCMenuItemSpriteExtra::create(
        GroundColorSelectBtnSprite,
        nullptr,
        this,
        menu_selector(RoomSettingsPopup::openGroundColorSelect)
    );
    menuG->addChild(GroundColorSelectBtn);

    menuG->updateLayout();
    menuBG->updateLayout();

    auto BGLabel = CCLabelBMFont::create("Background", "bigFont.fnt");
    BGLabel->setAnchorPoint({.5f, 0});
    BGLabel->setPosition(menuBG->getPosition() + menuBG->getContentSize() / 2 + ccp(0, 5));
    BGLabel->setScale(.6f);
    m_mainLayer->addChild(BGLabel);

    auto GLabel = CCLabelBMFont::create("Ground", "bigFont.fnt");
    GLabel->setAnchorPoint({.5f, 0});
    GLabel->setScale(.6f);
    GLabel->setPosition(menuG->getPosition() + ccp(-menuG->getContentWidth() / 2, menuG->getContentHeight() / 2 + 5));
    m_mainLayer->addChild(GLabel);

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
    colorPicker->setCallback([&](auto newColor){
        updateColor(newColor);
    });
    colorPicker->show();
    lastClickedColorBtn = 1;
}

void RoomSettingsPopup::openGroundColorSelect(CCObject*){
    ccColor3B currGroundColor = Save::getGroundColor();
    auto colorPicker = ColorPickPopup::create(currGroundColor);
    colorPicker->setCallback([&](auto newColor){
        updateColor(newColor);
    });
    colorPicker->show();
    lastClickedColorBtn = 2;
}
