#include "CatsLayer.hpp"

#include "../nodes/Cat.hpp"

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

    auto completedLevels = GameLevelManager::get()->getCompletedLevels(true);

    std::vector<GJGameLevel*> extremes{};

    for (const auto& level : CCArrayExt<GJGameLevel*>(completedLevels))
    {
        if (level->m_demonDifficulty == 6 && level->m_difficulty == GJDifficulty::Auto) extremes.push_back(level);
    }

    if (!extremes.size()) return false;

    auto winSize = CCDirector::get()->getWinSize();

    ScrollNode = ScrollLayer::create(winSize, true, false);
    this->addChild(ScrollNode);

    ScrollNode->m_contentLayer->setContentWidth(1600);

    float overallSize = 0;

    while (overallSize < ScrollNode->m_contentLayer->getContentWidth())
    {
        auto BG = CCSprite::create("game_bg_01_001.png");
        BG->setColor({ 19, 120, 179 });
        BG->setPositionX(overallSize);
        BG->setAnchorPoint({0, 0});
        BG->setZOrder(-10);
        ScrollNode->m_contentLayer->addChild(BG);
        overallSize += BG->getContentWidth();
    }

    overallSize = 0;
    float floorLength = 0;

    do
    {
        auto floor = CCSprite::create("groundSquare_05_001.png");
        floor->setSkewX(60);
        floor->setColor({ 48, 100, 129 });
        floor->setPositionX(overallSize);
        floor->setPositionY(15.1f);
        floor->setAnchorPoint({1, .5f});
        floor->setScaleY(.75f);
        ScrollNode->m_contentLayer->addChild(floor);
        floorLength = floor->getContentWidth();

        auto line = CCSprite::createWithSpriteFrameName("blockOutline_14new_001.png");
        line->setPosition(ccp(floor->getContentWidth() / 2 + overallSize, floor->getPositionY() +  floor->getScaledContentHeight() / 2));
        line->setSkewX(60);
        line->setZOrder(1);
        line->setScaleX(7.4f);
        ScrollNode->m_contentLayer->addChild(line);
        overallSize += floor->getContentWidth();
    } while (overallSize < ScrollNode->m_contentLayer->getContentWidth() + floorLength * 2);

    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    for (int i = 0; i < 15; i++)
    {
        auto tesCat = Cat::create(ScrollNode->m_contentLayer);
        ScrollNode->m_contentLayer->addChild(tesCat);
    }

    scheduleUpdate();

    return true;
}

void CatsLayer::keyBackClicked(){
    CCDirector::get()->popSceneWithTransition(.5f, PopTransition::kPopTransitionFade);
}

void CatsLayer::update(float dt){

    if (-ScrollNode->m_contentLayer->getContentWidth() + ScrollNode->getContentWidth() > 0)
        ScrollNode->m_contentLayer->setPositionX(0);
    else
        ScrollNode->m_contentLayer->setPositionX(std::clamp(ScrollNode->m_contentLayer->getPositionX(), -ScrollNode->m_contentLayer->getContentWidth() + ScrollNode->getContentWidth(), 0.0f));
}