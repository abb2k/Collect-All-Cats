#include "CatsLayer.hpp"

#include <nodes/popup/RoomSettingsPopup.hpp>
#include <utils/Save.hpp>
#include <utils/Utils.hpp>
#include <kittyAI/CatWanderState.hpp>
#include <kittyAI/CatIdleState.hpp>
#include <layers/NewCatsLayer.hpp>

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

    CCSize size = winSize;
    size.width = 1600;

    ScrollNode = AdvancedScrollLayer::create(winSize, size + ccp(1, 1));
    ScrollNode->zoomToMinimum();
    ScrollNode->scrollMovement = false;
    this->addChild(ScrollNode);

    int currGround = Save::getGround();
    ccColor3B currGroundColor = Save::getGroundColor();
    
    groundLoader = GroundLoader::create(ScrollNode->content->getContentWidth(), currGround, 2);
    groundLoader->setColor(currGroundColor);
    groundLoader->setContentHeight(1);
    ScrollNode->content->addChild(groundLoader);

    int currBG = Save::getBackground();
    ccColor3B currBGColor = Save::getBackgroundColor();
    
    bgLoader = BGLoader::create(ScrollNode->content->getContentWidth(), currBG);
    bgLoader->setColor(currBGColor);
    bgLoader->setZOrder(-10);
    bgLoader->setContentHeight(1);
    ScrollNode->content->addChild(bgLoader);

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

    catContainer = CCNode::create();
    catContainer->setID("cat-container");
    catContainer->setContentSize(ScrollNode->content->getContentSize());
    catContainer->setContentHeight(55);
    ScrollNode->content->addChild(catContainer);

    for (const auto& level : beatenExtremes)
        addCat(level);

    scheduleUpdate();

    auto didLoadCat = Save::loadCat(beatenExtremes[0]);

    auto newCats = NewCatsLayer::create({didLoadCat.unwrap()});
    this->addChild(newCats);

    return true;
}

void CatsLayer::keyBackClicked(){
    CCDirector::get()->popSceneWithTransition(.5f, PopTransition::kPopTransitionFade);
    catSettingsNode->hide();
    sharedInstance = nullptr;
}

void CatsLayer::update(float dt){

    if (followTarget != nullptr)
        followUpdate(dt);
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

    auto cat = Cat::create(catContainer, catLevel);
    if (cat == nullptr) return;
    cat->setPositionY(20);
    cat->setPositionX(Utils::GetRandomFloat(0, ScrollNode->content->getContentWidth() - cat->getScaledContentWidth()));
    cat->addAIState("wander", CatWanderState::create({1, 3}, {10, 25}));
    cat->addAIState("idle", CatIdleState::create({1, 4}));
    cat->addAIStateTransition("wander", "wander");
    cat->addAIStateTransition("idle", "idle");
    cat->addAIStateTransition("idle", "wander");
    cat->addAIStateTransition("wander", "idle");
    cat->setDefaultState(Utils::GetRandomInt(0, 1) == 0 ? "idle" : "wander");
    cat->startAI();
    catContainer->addChild(cat);
    spawnedCats.insert({catLevel->m_levelID.value(), cat});
}

void CatsLayer::removeCat(int catID){
    if (!spawnedCats.contains(catID)) return;

    if (spawnedCats[catID] == followTarget)
        followTarget = nullptr;

    Cat* cat = nullptr;
    cat = spawnedCats[catID];
    cat->removeMeAndCleanup();
    spawnedCats.erase(catID);
}

void CatsLayer::createCatSettingsNode(CCScene* scene){
    if (catSettingsNode != nullptr) return;
    catSettingsNode = CatSettingsLayer::create();
    catSettingsNode->setScale(.6f);
    catSettingsNode->setPosition({658, 60});
    scene->addChild(catSettingsNode, 100);
}

void CatsLayer::setFollowTarget(Cat* cat){
    followTarget = cat;

    for (const auto& [catID, catObj] : spawnedCats)
    {
        if (followTarget == nullptr){
            catObj->runAction(CCFadeTo::create(.2f, 255));
        }
        else{
            if (catObj != followTarget)
                catObj->runAction(CCFadeTo::create(.2f, 25));
            else
                catObj->runAction(CCFadeTo::create(.2f, 255));
        }
    }
}

void CatsLayer::followUpdate(float dt){

    auto cameraPos = (ScrollNode->content->getPosition() + ScrollNode->getAccurateContentSize()) * -1;

    CCPoint destination = ScrollNode->content->getParent()->convertToNodeSpace(ScrollNode->content->convertToWorldSpace(cameraPos - followTarget->getPosition())) + ScrollNode->getContentSize() / 2 / ScrollNode->getCurrentZoom();
    CCPoint lerpedMovement;

    lerpedMovement.x = std::lerp(ScrollNode->content->getPositionX(), destination.x, dt * 5);
    lerpedMovement.y = std::lerp(ScrollNode->content->getPositionY(), destination.y, dt * 5);

    //log::info("{} | {} | {}", ScrollNode->content->getPosition(), destination, dt);

    ScrollNode->moveTo(lerpedMovement);
}

Cat* CatsLayer::getCatFromStats(const CatStats& stats){
    auto tempstats = stats;
    auto relatedLevel = tempstats.getLevel();

    if (relatedLevel == nullptr) return nullptr;
    
    auto levelID = relatedLevel->m_levelID.value();
    
    return !spawnedCats.contains(levelID) ? nullptr : spawnedCats[levelID];
}