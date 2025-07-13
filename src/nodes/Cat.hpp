#pragma once

#include <Geode/Geode.hpp>
#include "../utils/Utils.hpp"
#include "../types/CatStats.hpp"

using namespace geode::prelude;

class CatAIStateBase;

class Cat : public CCNode {
public:
    static Cat* create(CCNode* wanderArea, GJGameLevel* relatedLevel);

    void updateSize();

    CatStats getStats();

    bool movementAllowed = true;

    void setCatStats(const CatStats& newStats);

private:
    virtual bool init(CCNode* wanderArea, GJGameLevel* relatedLevel);

    CatStats stats = CatStats::createEmpty();

    CCNode* wanderArea;

    void onCatClicked(CCObject*);

    std::vector<CatAIStateBase*> possibleAIStates{};

    CCSprite* kittyColonThreeSprite;

    void ChangeCatWanderState();

    void update(float dt);

    CCLabelBMFont* nameLabel;
    CCLabelBMFont* levelNameLabel;
};