#include "CatSettingsLayer.hpp"
#include "CatsLayer.hpp"

CatSettingsLayer* CatSettingsLayer::create() {
    auto ret = new CatSettingsLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CatSettingsLayer::init() {
    CCTouchDispatcher::get()->registerForcePrio(this, 3);
    if (!CCLayer::init()) return false;

    auto winSize = CCDirector::get()->getWinSize();

    this->setContentSize({250, winSize.height});
    this->setPositionX(winSize.width);
    this->setAnchorPoint({0, 0});

    auto BG = CCScale9Sprite::create("geode.loader/GE_square03.png");
    BG->setAnchorPoint({0, 0});
    BG->setContentSize(this->getContentSize());
    this->addChild(BG);

    buttonsMenu = CCMenu::create();
    buttonsMenu->setContentSize(this->getContentSize() - ccp(30, 0));
    buttonsMenu->setPosition({0, 0});
    buttonsMenu->setZOrder(100);
    buttonsMenu->setEnabled(false);
    buttonsMenu->setTouchPriority(-1);
    this->addChild(buttonsMenu);

    auto backBtnSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    backBtnSprite->setScale(0.75f);
    backBtnSprite->setFlipX(true);
    auto backBtn = CCMenuItemSpriteExtra::create(
        backBtnSprite,
        nullptr,
        this,
        menu_selector(CatSettingsLayer::onBackClicked)
    );
    backBtn->setPosition({backBtn->getContentWidth() / 2 + 10, buttonsMenu->getContentHeight() - backBtn->getContentHeight() / 2 - 5});
    buttonsMenu->addChild(backBtn);

    catDisplay = LinkedCatDisplay::create(nullptr);
    catDisplay->setScale(110.0f / catDisplay->getContentHeight());
    catDisplay->setPosition({buttonsMenu->getContentWidth() / 2, buttonsMenu->getContentHeight() - catDisplay->getScaledContentHeight() / 2 - 10});
    this->addChild(catDisplay);

    auto sizeLabel = CCLabelBMFont::create("Size", "bigFont.fnt");
    sizeLabel->setScale(0.75f);
    sizeLabel->setPosition({buttonsMenu->getContentWidth() / 2, 155});
    this->addChild(sizeLabel);
    sizeScroll = Slider::create(this, menu_selector(CatSettingsLayer::onSizeValueChanged), .75f);
    sizeScroll->setPosition({buttonsMenu->getContentWidth() - sizeScroll->m_groove->getScaledContentWidth() / 2 - sizeScroll->m_sliderBar->getScaledContentWidth() / 4, 132});
    this->addChild(sizeScroll);
    sizeInputField = TextInput::create(45, "S", "bigFont.fnt");
    sizeInputField->setPosition(ccp(30, sizeScroll->getPositionY()));
    sizeInputField->setCommonFilter(CommonFilter::Float);
    sizeInputField->setCallback([&](const std::string& newText){
        auto numRes = geode::utils::numFromString<float>(newText);
        if (numRes.isErr()) return;
        float realNum = numRes.unwrap();

        if (realNum < 0.5f){
            realNum = 0.5f;
        }
        if (realNum > 2.0f){
            realNum = 2.0f;
        }

        sizeScroll->setValue((realNum - 0.5f) / (2.0f - 0.5f));
        catToModify.size = realNum;
    });
    this->addChild(sizeInputField);

    auto nameLabel = CCLabelBMFont::create("Name", "bigFont.fnt");
    nameLabel->setScale(0.75f);
    nameLabel->setPosition({buttonsMenu->getContentWidth() / 2, 105});
    this->addChild(nameLabel);
    nameInputField = TextInput::create(200, "name", "bigFont.fnt");
    nameInputField->setPosition({buttonsMenu->getContentWidth() / 2, 78});
    nameInputField->setCommonFilter(CommonFilter::Name);
    nameInputField->setCallback([&](const std::string& newText){
        catToModify.name = newText;
    });
    this->addChild(nameInputField);

    auto applyBtnSprite = ButtonSprite::create("Apply Changes", "bigFont.fnt", "GJ_button_01.png");
    applyBtnSprite->setScale(.65f);
    auto applyBtn = CCMenuItemSpriteExtra::create(
        applyBtnSprite,
        nullptr,
        this,
        menu_selector(CatSettingsLayer::applyChanges)
    );
    applyBtn->setPosition({buttonsMenu->getContentWidth() / 2, 30});
    buttonsMenu->addChild(applyBtn);

    auto blockerMenu = CCMenu::create();
    blockerMenu->setContentSize(this->getContentSize() - ccp(30, 0));
    blockerMenu->setPosition({0, 0});
    blockerMenu->setZOrder(100);
    blockerMenu->setEnabled(true);
    this->addChild(blockerMenu);

    auto blocker = CCMenuItem::create();
    blocker->setContentSize(blockerMenu->getContentSize());
    blocker->setAnchorPoint({0, 0});
    blockerMenu->addChild(blocker);

    this->setTouchEnabled(false);
    this->setKeyboardEnabled(false);
    this->setKeypadEnabled(false);
    
    return true;
}

void CatSettingsLayer::show(){
    if (isOpen) return;
    isOpen = true;
    this->runAction(CCEaseBackOut::create(CCMoveBy::create(0.4f, ccp(-this->getContentWidth() + 30, 0))));

    this->setTouchEnabled(true);
    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);
    buttonsMenu->setEnabled(true);
    handleTouchPriorityWith(this, -600, true);

    auto selPopup = CatsLayer::activeCatLayer()->currentSelectionPopup;
    if (!selPopup) return;

    selPopup->easeHorizontal(CCMoveBy::create(0.3f, ccp(-60, 0)));
}

void CatSettingsLayer::hide(){
    if (!isOpen) return;
    isOpen = false;
    this->runAction(CCEaseBackIn::create(CCMoveBy::create(0.4f, ccp(this->getContentWidth() - 30, 0))));

    this->setTouchEnabled(false);
    this->setKeyboardEnabled(false);
    this->setKeypadEnabled(false);
    buttonsMenu->setEnabled(false);

    auto selPopup = CatsLayer::activeCatLayer()->currentSelectionPopup;
    if (!selPopup) return;

    selPopup->easeHorizontal(CCMoveBy::create(0.3f, ccp(60, 0)));
}

void CatSettingsLayer::keyBackClicked(){
    hide();
}

void CatSettingsLayer::setToCat(const CatStats& stats){
    catToModify = stats;
    catDisplay->setCat(stats);
    catDisplay->setScale(110.0f / catDisplay->getContentHeight());
    catDisplay->setPosition({buttonsMenu->getContentWidth() / 2, buttonsMenu->getContentHeight() - catDisplay->getScaledContentHeight() / 2 - 10});
    sizeScroll->setValue((stats.size - 0.5f) / (2.0f - 0.5f));
    sizeInputField->setString(fmt::format("{:.2f}", stats.size));
    nameInputField->setString(stats.name);
}

void CatSettingsLayer::onCatApplyCallback(const std::function<void(const CatStats&)>& callback){
    this->callback = callback;
}

void CatSettingsLayer::onBackClicked(CCObject*){
    keyBackClicked();
}

void CatSettingsLayer::onSizeValueChanged(CCObject*){
    catToModify.size = 0.5f + sizeScroll->getValue() * (2.0f - 0.5f);
    sizeInputField->setString(fmt::format("{:.2f}", catToModify.size));
}

void CatSettingsLayer::applyChanges(CCObject*){
    if (callback == NULL) return;

    callback(catToModify);
}