#include "CatSettingsLayer.hpp"

#include <layers/CatsLayer.hpp>
#include <utils/Save.hpp>

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

    // @geode-ignore(unknown-resource)
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

    auto sizeLabel = CCLabelBMFont::create("Size", "bigFont.fnt");
    sizeLabel->setScale(0.75f);
    sizeLabel->setPosition({buttonsMenu->getContentWidth() / 2, 155 - 50});
    this->addChild(sizeLabel);
    sizeScroll = Slider::create(this, menu_selector(CatSettingsLayer::onSizeValueChanged), .75f);
    sizeScroll->setPosition({buttonsMenu->getContentWidth() - sizeScroll->m_groove->getScaledContentWidth() / 2 - sizeScroll->m_sliderBar->getScaledContentWidth() / 4, 132 - 50});
    this->addChild(sizeScroll);
    sizeInputField = TextInput::create(45, "S", "bigFont.fnt");
    sizeInputField->setPosition(ccp(30, sizeScroll->getPositionY()));
    sizeInputField->setCommonFilter(CommonFilter::Float);
    sizeInputField->setCallback([&](const std::string& newText){
        auto numRes = geode::utils::numFromString<float>(newText);
        if (numRes.isErr()) return;
        float realNum = numRes.unwrap();

        if (realNum < CatStats::MIN_SIZE)
            realNum = CatStats::MIN_SIZE;

        if (realNum > CatStats::MAX_SIZE)
            realNum = CatStats::MAX_SIZE;

        sizeScroll->setValue((realNum - CatStats::MIN_SIZE) / (CatStats::MAX_SIZE - CatStats::MIN_SIZE));
        catToModify.size = realNum;

        updateLivingCat();
    });
    this->addChild(sizeInputField);

    auto nameLabel = CCLabelBMFont::create("Name", "bigFont.fnt");
    nameLabel->setScale(0.75f);
    nameLabel->setPosition({buttonsMenu->getContentWidth() / 2, 60});
    this->addChild(nameLabel);
    nameInputField = TextInput::create(200, "name", "bigFont.fnt");
    nameInputField->setPosition({buttonsMenu->getContentWidth() / 2, 78 - 50});
    nameInputField->setCommonFilter(CommonFilter::Name);
    nameInputField->setCallback([&](const std::string& newText){
        catToModify.name = newText;

        nameResetBtn->setVisible(catToModify.name != catToModify.getLevel()->m_levelName);

        updateLivingCat();
    });
    this->addChild(nameInputField);

    auto nameResetBtnSpr = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
    nameResetBtnSpr->setScale(.4f);
    nameResetBtn = CCMenuItemSpriteExtra::create(
        nameResetBtnSpr,
        this,
        menu_selector(CatSettingsLayer::nameReset)
    );
    nameResetBtn->setPosition({158, 58});
    buttonsMenu->addChild(nameResetBtn);

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

    if (catToModify.getLevel() != nullptr)
        nameResetBtn->setVisible(catToModify.name != catToModify.getLevel()->m_levelName);

    selPopup->fadeTo(10, 0.3f);
}

void CatSettingsLayer::hide(){
    if (!isOpen) return;
    isOpen = false;
    this->runAction(CCEaseBackIn::create(CCMoveBy::create(0.4f, ccp(this->getContentWidth() - 30, 0))));

    CatsLayer::activeCatLayer()->setFollowTarget(nullptr);

    this->setTouchEnabled(false);
    this->setKeyboardEnabled(false);
    this->setKeypadEnabled(false);
    buttonsMenu->setEnabled(false);

    auto selPopup = CatsLayer::activeCatLayer()->currentSelectionPopup;
    if (!selPopup) return;

    selPopup->fadeTo(255, 0.3f);
}

void CatSettingsLayer::keyBackClicked(){
    hide();
}

void CatSettingsLayer::setToCat(CatStats& stats){
    catToModify = stats;
    sizeScroll->setValue((stats.size - CatStats::MIN_SIZE) / (CatStats::MAX_SIZE - CatStats::MIN_SIZE));
    sizeInputField->setString(fmt::format("{:.2f}", stats.size));
    nameInputField->setString(stats.name);

    auto catsLayer = CatsLayer::activeCatLayer();

    catsLayer->setFollowTarget(catsLayer->getCatFromStats(stats));

    if (catToModify.getLevel() != nullptr)
        nameResetBtn->setVisible(catToModify.name != catToModify.getLevel()->m_levelName);
}

void CatSettingsLayer::onBackClicked(CCObject*){
    keyBackClicked();
}

void CatSettingsLayer::onSizeValueChanged(CCObject*){
    catToModify.size = CatStats::MIN_SIZE + sizeScroll->getValue() * (CatStats::MAX_SIZE - CatStats::MIN_SIZE);
    sizeInputField->setString(fmt::format("{:.2f}", catToModify.size));

    updateLivingCat();
}

void CatSettingsLayer::updateLivingCat(){
    auto livingCatUpdate = [&]() -> bool {
        auto catLayer = CatsLayer::activeCatLayer();
        if (catLayer == nullptr) return false;

        auto cat = catLayer->getCatFromStats(catToModify);
        if (cat == nullptr) return false;

        cat->setCatStats(catToModify);

        return true;
    };
    
    if (!livingCatUpdate()){
        auto _ = Save::saveCat(&catToModify);
    }
}

void CatSettingsLayer::showWithCat(CatStats& stats){
    CatsLayer::activeCatLayer()->catSettingsNode->setToCat(stats);
    CatsLayer::activeCatLayer()->catSettingsNode->show();
}

void CatSettingsLayer::nameReset(CCObject*){
    if (catToModify.getLevel() != nullptr)
        catToModify.name = catToModify.getLevel()->m_levelName;

    nameInputField->setString(catToModify.name);

    nameResetBtn->setVisible(false);

    updateLivingCat();
}