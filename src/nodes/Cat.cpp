#include "Cat.hpp"

#include <utils/Utils.hpp>
#include <utils/Save.hpp>
#include <layers/CatsLayer.hpp>
#include <kittyAI/CatAIStateBase.hpp>
#include <utils/Utils.hpp>

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

    auto didLoadCat = Save::loadCat(relatedLevel);
    if (didLoadCat.isErr()){
        stats = CatStats::createDefault(relatedLevel);

        float randomSizeOffset = .2f;

        stats.size = Utils::GetRandomFloat(CatStats::MIN_SIZE + randomSizeOffset, CatStats::MAX_SIZE - randomSizeOffset);

        auto didSaveWork = Save::saveCat(this);
        if (didSaveWork.isErr()){
            log::error("Failed to load cat!\n{}", didSaveWork.unwrapErr());
            return false;
        }
    }
    else stats = didLoadCat.unwrap();

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    menu->setID("button-menu");
    this->addChild(menu);

    this->setContentSize({60, 60});
    this->setAnchorPoint({.5f, 0});

    auto btn = CCMenuItem::create(this, menu_selector(Cat::onCatClicked));
    btn->setContentSize(this->getContentSize());
    btn->setPosition(this->getContentSize() / 2);
    btn->setID("general-kitty-btn");
    menu->addChild(btn);

    kittyColonThreeSprite = CCSprite::createWithSpriteFrameName("default_cat.png"_spr);
    kittyColonThreeSprite->setPosition(this->getContentSize() / 2);
    kittyColonThreeSprite->setScale(.7f);
    this->addChild(kittyColonThreeSprite);
    
    updateSize();

    nameLabel = CCLabelBMFont::create(stats.name.c_str(), "bigFont.fnt");
    nameLabel->setPosition({this->getContentWidth() / 2, 80});
    nameLabel->setScale(.5f);
    this->addChild(nameLabel);
    levelNameLabel = CCLabelBMFont::create(stats.getLevel()->m_levelName.c_str(), "goldFont.fnt");
    levelNameLabel->setPosition({this->getContentWidth() / 2, 70});
    levelNameLabel->setScale(.25f);
    this->addChild(levelNameLabel);

    if (stats.name == stats.getLevel()->m_levelName)
        levelNameLabel->setString("");

    this->scheduleUpdate();

    return true;
}

void Cat::onCatClicked(CCObject*){
    kittyColonThreeSprite->runAction(CCSequence::create(CCTintTo::create(0, 0, 255, 0), CCTintTo::create(0.5f, 255, 255, 255), nullptr));

    CatsLayer::activeCatLayer()->catSettingsNode->setToCat(stats);
    CatsLayer::activeCatLayer()->catSettingsNode->show();
    CatsLayer::activeCatLayer()->catSettingsNode->onCatApplyCallback(std::bind(&Cat::setCatStats, this, std::placeholders::_1));
}

void Cat::updateSize(){
    this->setZOrder(1000 / stats.size + 1);
    this->setScale(stats.size);
}

CatStats Cat::getStats() { return stats; }

void Cat::setCatStats(const CatStats& newStats){
    stats = newStats;

    updateSize();

    nameLabel->setString(stats.name.c_str());

    if (stats.name == stats.getLevel()->m_levelName)
        levelNameLabel->setString("");
    else
        levelNameLabel->setString(stats.getLevel()->m_levelName.c_str());

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

        Cat::moveToState(possibleOthers[Utils::GetRandomInt(0, possibleOthers.size() - 1)]);
    });
}

void Cat::update(float dt){
    Cat::AIUpdate(dt);
}

void Cat::AIUpdate(float dt){
    if (currentAIState != nullptr)
        currentAIState->update(dt);
}

CCNode* Cat::getVisualParent() { return kittyColonThreeSprite; }