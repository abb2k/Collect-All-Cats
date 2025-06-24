#include "Cat.hpp"

#include "../utils/Save.hpp"
#include "../layers/CatsLayer.hpp"


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
    if (!CCNode::init()) return false;

    this->wanderArea = wanderArea;

    auto didLoadCat = Save::loadCat(relatedLevel);
    if (didLoadCat.isErr()){
        stats = CatStats::createDefault(relatedLevel);
        auto didSaveWork = Save::saveCat(this);
        if (didSaveWork.isErr()){
            log::info("Failed to load cat!\n{}", didSaveWork.unwrapErr());
            return false;
        }
    }
    else stats = didLoadCat.unwrap();

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    menu->setID("button-menu");
    this->addChild(menu);

    this->setContentSize({60, 60});

    this->setSize(1);

    auto btn = CCMenuItem::create(this, menu_selector(Cat::OnCatClicked));
    btn->setContentSize(this->getContentSize());
    btn->setPosition(this->getContentSize() / 2);
    btn->setID("general-kitty-btn");
    menu->addChild(btn);

    kittyColonThreeSprite = CCSprite::createWithSpriteFrameName("colourPickerShadow.png");
    kittyColonThreeSprite->setPosition(this->getContentSize() / 2);
    kittyColonThreeSprite->setScale(.7f);
    this->addChild(kittyColonThreeSprite);

    scheduleUpdate();

    return true;
}

void Cat::OnCatClicked(CCObject*){
    kittyColonThreeSprite->runAction(CCSequence::create(CCTintTo::create(0, 0, 255, 0), CCTintTo::create(0.5f, 255, 255, 255), nullptr));

    CatsLayer::activeCatLayer()->catSettingsNode->setToCat(stats);
    CatsLayer::activeCatLayer()->catSettingsNode->show();
}

void Cat::ChangeCatWanderState(){

    if (currentWanderState == CatWanderStates::Idle){
        int decision = Utils::GetRandomInt(0, 1);
        if (decision == 0){
            currentWanderState = CatWanderStates::Sleeping;

            stateChangeTimer = Utils::GetRandomFloat(5, 10);

            kittyColonThreeSprite->setScale(.5f);
        }
        else{
            currentWanderState = CatWanderStates::Walking;

            stateChangeTimer = Utils::GetRandomFloat(4, 10);
            walkDirection = Utils::GetRandomFloat(-1, 1);
        }
    }
    else{
        stateChangeTimer = Utils::GetRandomFloat(2, 5);
        currentWanderState = CatWanderStates::Idle;
        kittyColonThreeSprite->setScale(.7f);
    }
}

void Cat::update(float dt){
    if (stateChangeTimer > 0)
        stateChangeTimer -= dt;
    else
        ChangeCatWanderState();

    if (currentWanderState == CatWanderStates::Walking && movementAllowed){
        setPositionX(std::clamp(getPositionX() + walkDirection * 0.25f, 0.0f, wanderArea->getContentWidth() - getScaledContentWidth()));
    }
}

void Cat::setSize(float newSize){
    newSize = std::clamp(newSize, 0.5f, 3.0f);
    this->setZOrder(100 / stats.size + 1);
    this->setScale(stats.size);
}

CatStats Cat::getStats() { return stats; }