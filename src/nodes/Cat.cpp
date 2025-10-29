#include "Cat.hpp"

#include <utils/CatUtils.hpp>
#include <utils/Save.hpp>
#include <layers/CatsLayer.hpp>
#include <kittyAI/CatAIStateBase.hpp>
#include <utils/CatUtils.hpp>

Cat* Cat::create(CCNode* wanderArea, GJGameLevel* relatedLevel) {
    auto ret = new Cat();
    if (ret->init(wanderArea, relatedLevel)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Cat::init(CCNode* wanderArea, GJGameLevel* relatedLevel) {
    if (!CCMenu::init()) return false;

    this->wanderArea = wanderArea;

    ignoreAnchorPointForPosition(false);

    auto didLoadCat = Save::loadCatOrDefault(relatedLevel);
    
    if (didLoadCat.isErr()) log::error("{}", didLoadCat.unwrapErr());
    else stats = didLoadCat.unwrap();
    
    stats.setOnAREDLStatsRecievedCallback([this](CatStats*){
        log::info("{}", stats.getLevelDetails()->position);
    });
    stats.loadAREDLLevelData();

    this->setContentSize({60, 60});
    this->setAnchorPoint({.5f, 0});
    this->setScale(stats.size);

    auto btn = CCMenuItem::create(this, menu_selector(Cat::onCatClicked));
    btn->setContentSize(this->getContentSize());
    btn->setPosition(this->getContentSize() / 2 + ccp(0, 10));
    btn->setID("general-interact-range");
    this->addChild(btn);

    visualParent = CatVisualDisplay::create();
    visualParent->setID("visual-parent");
    visualParent->setContentSize({0, 0});
    visualParent->setPosition(this->getContentSize() / 2 + ccp(0, 10));
    visualParent->ignoreAnchorPointForPosition(false);
    this->addChild(visualParent);
    
    nameLabel = CCLabelBMFont::create(stats.name.c_str(), "bigFont.fnt");
    nameLabel->setID("name-label");
    nameLabel->setPosition({this->getContentWidth() / 2, 80});
    nameLabel->setScale(.5f);
    this->addChild(nameLabel);
    levelNameLabel = CCLabelBMFont::create(stats.getLevel()->m_levelName.c_str(), "goldFont.fnt");
    levelNameLabel->setID("level-name-label");
    levelNameLabel->setPosition({this->getContentWidth() / 2, 70});
    levelNameLabel->setScale(.25f);
    this->addChild(levelNameLabel);

    getVisualParent()->setRotationY(CatUtils::GetRandomInt(0, 1) == 0 ? 180 : 0);

    if (stats.name == stats.getLevel()->m_levelName)
        levelNameLabel->setString("");

    this->scheduleUpdate();

    Cat::setCatStats(stats);

    return true;
}

void Cat::onCatClicked(CCObject*){
    visualParent->runAction(CCSequence::create(CCTintTo::create(0, 0, 255, 0), CCTintTo::create(0.5f, 255, 255, 255), nullptr));

    CatsLayer::activeCatLayer()->catSettingsNode->showWithCat(stats);
}

CatStats Cat::getStats() { return stats; }

void Cat::setCatStats(const CatStats& newStats){
    stats = newStats;

    this->setScale(stats.size);

    nameLabel->setString(stats.name.c_str());

    if (stats.name == stats.getLevel()->m_levelName)
        levelNameLabel->setString("");
    else
        levelNameLabel->setString(stats.getLevel()->m_levelName.c_str());

    visualParent->updateVisuals(stats);

    auto _ = Save::saveCat(&stats);
}

void Cat::addAIState(const std::string& stateName, std::shared_ptr<CatAIStateBase> state){
    if (possibleAIStates.contains(stateName)) return;

    state->setTargetCat(this);
    
    state->name = stateName;
    possibleAIStates.insert({stateName, state});
}
void Cat::addAIStateTransition(const std::string& originStateName, const std::string& targetStateName){
    if (!possibleAIStates.contains(originStateName) || !possibleAIStates.contains(targetStateName)) return;

    if (!AIStatesTransitions.contains(originStateName))
        AIStatesTransitions[targetStateName] = {};

    for (const auto& existingTarget : AIStatesTransitions[originStateName])
        if (existingTarget == targetStateName) return;
    
    AIStatesTransitions[originStateName].push_back(targetStateName);
}

void Cat::setDefaultState(const std::string& stateName){
    defaultState = stateName;
}

void Cat::startAI(){
    if (!possibleAIStates.contains(defaultState)) return;

    Cat::moveToState(defaultState);
}

void Cat::stopAI(){
    if (currentAIState != nullptr)
        currentAIState->endState();
    currentAIState = nullptr;
}

void Cat::moveToState(const std::string& stateName){
    if (!possibleAIStates.contains(stateName)) return;

    if (currentAIState != nullptr)
        currentAIState->endState();

    currentAIState = possibleAIStates[stateName];

    currentAIState->startState();
    currentAIState->onStateEndedCallback([&](CatAIStateBase* state){
        if (!AIStatesTransitions.contains(state->name)) return;

        auto possibleOthers = AIStatesTransitions[state->name];

        Cat::moveToState(possibleOthers[CatUtils::GetRandomInt(0, possibleOthers.size() - 1)]);
    });
}

void Cat::update(float dt){
    Cat::AIUpdate(dt);

    this->setZOrder(-static_cast<int>(this->getPositionY()));

    this->setPositionX(std::clamp(this->getPositionX(), this->getScaledContentWidth() * this->getAnchorPoint().x, this->wanderArea->getContentWidth() - this->getScaledContentWidth() * this->getAnchorPoint().x));
    this->setPositionY(std::clamp(this->getPositionY(), this->getScaledContentHeight() * this->getAnchorPoint().y, this->wanderArea->getContentHeight() - this->getScaledContentHeight() * this->getAnchorPoint().y));
}

void Cat::AIUpdate(float dt){
    if (currentAIState != nullptr)
        currentAIState->update(dt);
}

CCNode* Cat::getVisualParent() { return visualParent; }