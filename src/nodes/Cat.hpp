#pragma once

#include <Geode/Geode.hpp>
#include "../utils/Utils.hpp"

using namespace geode::prelude;

struct CatStats{
    std::string name;
    GJGameLevel* relatedLevel;
    unsigned int catTypeID;
    float size = 1;

    static CatStats createDefault(GJGameLevel* relatedLevel){
        CatStats newStats;
        newStats.relatedLevel = relatedLevel;
        newStats.name = relatedLevel->m_levelName;
        newStats.catTypeID = Utils::GetRandomInt(0, 10);

        return newStats;
    }
};

class Cat : public CCNode {
public:
    static Cat* create(CCNode* wanderArea, GJGameLevel* relatedLevel);

    void setSize(float newSize);

    CatStats getStats();

    bool movementAllowed = true;

private:
    virtual bool init(CCNode* wanderArea, GJGameLevel* relatedLevel);

    CatStats stats;

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