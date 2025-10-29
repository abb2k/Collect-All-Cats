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

    this->setContentSize({250, winSize.height + 50});
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

    auto topBar = CCScale9Sprite::create("square02_small.png");
    topBar->setContentSize({200, 30});
    topBar->setPosition({110, 320});
    topBar->setOpacity(100);
    this->addChild(topBar);

    topBarMenu = CCMenu::create();
    topBarMenu->setContentSize(topBar->getContentSize());
    topBarMenu->setPosition(topBar->getPosition());
    topBarMenu->ignoreAnchorPointForPosition(false);
    topBarMenu->setLayout(SimpleAxisLayout::create(Axis::Row)
        ->setGap(5.0f)
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setCrossAxisScaling(AxisScaling::ScaleDownGaps)
        ->setMainAxisScaling(AxisScaling::ScaleDownGaps)
        ->setMinRelativeScale(.3f)
    );
    this->addChild(topBarMenu);

    auto centerContentBG = CCScale9Sprite::create("square02_small.png");
    centerContentBG->setContentSize({200, 175});
    centerContentBG->setPosition({110, 210});
    centerContentBG->setOpacity(100);
    this->addChild(centerContentBG);

    auto colorSkinsSwitchMenu = CCMenu::create();
    colorSkinsSwitchMenu->setContentSize({105, 17});
    colorSkinsSwitchMenu->setPosition({154, 282});
    colorSkinsSwitchMenu->setLayout(SimpleAxisLayout::create(Axis::Row)
        ->setGap(5.0f)
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setCrossAxisScaling(AxisScaling::ScaleDownGaps)
        ->setMainAxisScaling(AxisScaling::ScaleDownGaps)
        ->setMinRelativeScale(false)
    );
    this->addChild(colorSkinsSwitchMenu);

    auto skinsEnabledSpr = ButtonSprite::create("Skin", "bigFont.fnt", "GJ_button_03.png");
    skinsEnabledSpr->setID("enabled");
    skinsSwitchBtn = CCMenuItemSpriteExtra::create(
        skinsEnabledSpr,
        this,
        menu_selector(CatSettingsLayer::onColorSkinsSwitch)
    );
    auto skinsDisabledSpr = ButtonSprite::create("Skin", "bigFont.fnt", "GJ_button_04.png");
    skinsDisabledSpr->setID("disabled");
    skinsDisabledSpr->setPosition(skinsEnabledSpr->getPosition());
    skinsDisabledSpr->setVisible(false);
    skinsSwitchBtn->addChild(skinsDisabledSpr);
    colorSkinsSwitchMenu->addChild(skinsSwitchBtn);
    skinsSwitchBtn->setEnabled(false);

    auto colorEnabledSpr = ButtonSprite::create("Color", "bigFont.fnt", "GJ_button_03.png");
    colorEnabledSpr->setID("enabled");
    colorSwitchBtn = CCMenuItemSpriteExtra::create(
        colorEnabledSpr,
        this,
        menu_selector(CatSettingsLayer::onColorSkinsSwitch)
    );
    auto colorDisabledSpr = ButtonSprite::create("Color", "bigFont.fnt", "GJ_button_04.png");
    colorDisabledSpr->setID("disabled");
    colorDisabledSpr->setPosition(colorEnabledSpr->getPosition());
    colorSwitchBtn->addChild(colorDisabledSpr);
    colorSkinsSwitchMenu->addChild(colorSwitchBtn);
    colorEnabledSpr->setVisible(false);

    colorSkinsSwitchMenu->updateLayout();

    catagoryTitle = CCLabelBMFont::create("", "bigFont.fnt");
    catagoryTitle->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
    catagoryTitle->setAnchorPoint({0, .5f});
    catagoryTitle->setScale(.75f);
    catagoryTitle->setPosition({14, colorSkinsSwitchMenu->getPositionY() + 2});
    this->addChild(catagoryTitle);

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

    onCatagoryClicked(addCatagory("Cats", "cat", CCSprite::createWithSpriteFrameName("default_cat.png"_spr)));
    addCatagory("Hats", "hat", CCSprite::createWithSpriteFrameName("abb2k_cac_closed.png"_spr));
    
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

CCMenuItemSpriteExtra* CatSettingsLayer::addCatagory(const std::string& name, const std::string& resourceName, CCNode* visual){
    auto button = CCMenuItemSpriteExtra::create(
        visual,
        this,
        menu_selector(CatSettingsLayer::onCatagoryClicked)
    );
    button->setOpacity(150);
    topBarMenu->addChild(button);

    catagoriesMapped[button] = {name, resourceName};

    topBarMenu->updateLayout();

    return button;
}

void CatSettingsLayer::onCatagoryClicked(CCObject* sender){
    CCMenuItemSpriteExtra* button = nullptr;
    if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(sender)){button = btn;} else return;
    if (!catagoriesMapped.contains(button)) return;

    if (selectedPage != nullptr){
        selectedPage->setEnabled(true);
        selectedPage->setOpacity(150);
    }
    selectedPage = button;

    selectedPage->setEnabled(false);
    selectedPage->setOpacity(255);

    catagoryTitle->setString(catagoriesMapped[button].first.c_str());

    log::info("now in page {}", catagoriesMapped[button].first);
}

void CatSettingsLayer::onColorSkinsSwitch(CCObject* sender){
    if (skinsSwitchBtn == sender){
        currentEditingPage = 0;
        skinsSwitchBtn->setEnabled(false);
        colorSwitchBtn->setEnabled(true);

        skinsSwitchBtn->getChildByID("enabled")->setVisible(true);
        skinsSwitchBtn->getChildByID("disabled")->setVisible(false);

        colorSwitchBtn->getChildByID("enabled")->setVisible(false);
        colorSwitchBtn->getChildByID("disabled")->setVisible(true);
    }
    else if (colorSwitchBtn == sender){
        currentEditingPage = 1;
        skinsSwitchBtn->setEnabled(true);
        colorSwitchBtn->setEnabled(false);

        skinsSwitchBtn->getChildByID("enabled")->setVisible(false);
        skinsSwitchBtn->getChildByID("disabled")->setVisible(true);

        colorSwitchBtn->getChildByID("enabled")->setVisible(true);
        colorSwitchBtn->getChildByID("disabled")->setVisible(false);
    }
}