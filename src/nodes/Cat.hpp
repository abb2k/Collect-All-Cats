#pragma once

#include <Geode/Geode.hpp>
#include <types/CatStats.hpp>

using namespace geode::prelude;

class CatAIStateBase;

class Cat : public CCMenu {
public:
    static Cat* create(CCNode* wanderArea, GJGameLevel* relatedLevel);

    CatStats getStats();

    bool movementAllowed = true;

    void setCatStats(const CatStats& newStats);

    void addAIState(const std::string& stateName, std::shared_ptr<CatAIStateBase> state);
    void addAIStateTransition(const std::string& originStateName, const std::string& targetStateName);

    void setDefaultState(const std::string& stateName);

    void moveToState(const std::string& stateName);

    void startAI();
    void stopAI();

    void update(float dt) override;

    CCNode* getVisualParent();

private:
    virtual bool init(CCNode* wanderArea, GJGameLevel* relatedLevel);

    CatStats stats = CatStats::createEmpty();

    CCNode* wanderArea;

    void onCatClicked(CCObject*);

    std::map<std::string, std::shared_ptr<CatAIStateBase>> possibleAIStates{};
    std::map<std::string, std::vector<std::string>> AIStatesTransitions{};
    std::string defaultState;
    std::shared_ptr<CatAIStateBase> currentAIState = nullptr;
    void AIUpdate(float dt);

    CCMenu* visualParent;

    CCLabelBMFont* nameLabel;
    CCLabelBMFont* levelNameLabel;
};