#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Cat : public CCNode {
public:
    static Cat* create(CCNode* wanderArea);

private:
    virtual bool init(CCNode* wanderArea);

    std::string name;
    GJGameLevel* relatedLevel;
    int catTypeID;
    float size = 1;

    CCNode* wanderArea;

    void OnCatClicked(CCObject*);

    enum CatWanderStates{
        Walking,
        Idle,
        Sleeping
    };

    CatWanderStates currentWanderState;
    float walkDirection;
    float stateChangeTimer;

    CCSprite* kittyColonThreeSprite;

    void ChangeCatWanderState();

    void update(float dt);
};