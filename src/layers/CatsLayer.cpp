#include "CatsLayer.hpp"

#include "../nodes/popup/RoomSettingsPopup.hpp"
#include "../utils/Save.hpp"

CatsLayer* CatsLayer::sharedInstance = nullptr;

CatsLayer* CatsLayer::create() {
    auto ret = new CatsLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CatsLayer::init() {
    if (!CCLayer::init()) return false;

    if (sharedInstance == nullptr)
        sharedInstance = this;

    auto completedLevels = GameLevelManager::get()->getCompletedLevels(true);

    for (const auto& level : CCArrayExt<GJGameLevel*>(completedLevels))
    {
        if (level->m_demonDifficulty == 6 && level->m_difficulty == GJDifficulty::Auto) beatenExtremes.push_back(level);
    }

    if (!beatenExtremes.size()) return false;

    auto winSize = CCDirector::get()->getWinSize();

    ScrollNode = ScrollLayer::create(winSize, true, false);
    this->addChild(ScrollNode);

    ScrollNode->m_contentLayer->setContentWidth(1600);

    groundLoader = GroundLoader::create(ScrollNode->m_contentLayer->getContentWidth(), "groundSquare_05_001.png", 2);
    groundLoader->setColor({ 0, 102, 255 });
    groundLoader->setContentHeight(1);
    ScrollNode->m_contentLayer->addChild(groundLoader);

    bgLoader = BGLoader::create(ScrollNode->m_contentLayer->getContentWidth(), "game_bg_01_001.png");
    bgLoader->setColor({ 40, 125, 255 });
    bgLoader->setZOrder(-10);
    bgLoader->setContentHeight(1);
    ScrollNode->m_contentLayer->addChild(bgLoader);

    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    auto buttonsMenu = CCMenu::create();
    buttonsMenu->setID("buttons-menu");
    this->addChild(buttonsMenu);

    //exit button
    auto exitBtnSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    exitBtnSprite->setScale(.75f);
    auto exitBtn = CCMenuItemSpriteExtra::create(
        exitBtnSprite,
        nullptr,
        this,
        menu_selector(CatsLayer::onBackClicked)
    );
    exitBtn->setPosition({
        -buttonsMenu->getContentWidth() / 2 + exitBtn->getContentWidth() / 2 + 5,
        buttonsMenu->getContentHeight() / 2 - exitBtn->getContentHeight() / 2 - 5
    });
    buttonsMenu->addChild(exitBtn);
    
    auto topRightMenu = CCMenu::create();
    topRightMenu->setContentSize({250, 40});
    topRightMenu->setID("top-right-menu");
    topRightMenu->setPosition(winSize - ccp(5, 5));
    topRightMenu->setAnchorPoint({1, 1});
    topRightMenu->setLayout(SimpleAxisLayout::create(Axis::Row)
        ->setCrossAxisScaling(AxisScaling::ScaleDown)
        ->setMainAxisAlignment(MainAxisAlignment::End)
        ->setGap(5)
    );
    this->addChild(topRightMenu);

    //cats menu button
    auto catSelectBtnSprite = CCSprite::createWithSpriteFrameName("GJ_viewListsBtn_001.png");
    auto catSelectBtn = CCMenuItemSpriteExtra::create(
        catSelectBtnSprite,
        nullptr,
        this,
        menu_selector(CatsLayer::onCatsMenuClicked)
    );
    topRightMenu->addChild(catSelectBtn);

    //room settings button
    auto settingsBtnSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
    auto settingsBtn = CCMenuItemSpriteExtra::create(
        settingsBtnSprite,
        nullptr,
        this,
        menu_selector(CatsLayer::onSettingsClicked)
    );
    topRightMenu->addChild(settingsBtn);

    topRightMenu->updateLayout();

    for (const auto& level : beatenExtremes)
    {
        addCat(level);
    }

    scheduleUpdate();

    return true;
}

void CatsLayer::keyBackClicked(){
    CCDirector::get()->popSceneWithTransition(.5f, PopTransition::kPopTransitionFade);
    catSettingsNode->hide();
    sharedInstance = nullptr;
}

void CatsLayer::update(float dt){

    if (-ScrollNode->m_contentLayer->getContentWidth() + ScrollNode->getContentWidth() > 0)
        ScrollNode->m_contentLayer->setPositionX(0);
    else
        ScrollNode->m_contentLayer->setPositionX(std::clamp(ScrollNode->m_contentLayer->getPositionX(), -ScrollNode->m_contentLayer->getContentWidth() + ScrollNode->getContentWidth(), 0.0f));
}

void CatsLayer::onBackClicked(CCObject*) { keyBackClicked(); }

void CatsLayer::onCatsMenuClicked(CCObject*){
    currentSelectionPopup= CatSelectionPopup::create();
    if (!currentSelectionPopup) log::info("Failed to open cat selection menu!");
    currentSelectionPopup->show();
}

void CatsLayer::onSettingsClicked(CCObject*){
    RoomSettingsPopup::create()->show();
}

CatsLayer* CatsLayer::activeCatLayer(){
    return sharedInstance;
}

void CatsLayer::addCat(GJGameLevel* catLevel){
    auto placedCats = Save::getPlacedCats();
    std::set<int> placedCatsSet(placedCats.begin(), placedCats.end());

    if (!placedCatsSet.contains(catLevel->m_levelID.value())) return;

    auto cat = Cat::create(ScrollNode->m_contentLayer, catLevel);
    if (cat == nullptr) return;
    cat->setPositionY(20);
    cat->setPositionX(Utils::GetRandomFloat(0, ScrollNode->m_contentLayer->getContentWidth() - cat->getScaledContentWidth()));
    ScrollNode->m_contentLayer->addChild(cat);
    spawnedCats.insert({catLevel->m_levelID.value(), cat});
}

void CatsLayer::removeCat(int catID){
    if (!spawnedCats.contains(catID)) return;

    Cat* cat = nullptr;
    cat = spawnedCats[catID];
    cat->removeMeAndCleanup();
    spawnedCats.erase(catID);
}

void CatsLayer::createCatSettingsNode(CCScene* scene){
    if (catSettingsNode != nullptr) return;
    catSettingsNode = CatSettingsNode::create();
    scene->addChild(catSettingsNode, 100);
}