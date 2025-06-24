#pragma once

#include <Geode/Geode.hpp>
#include "Cat.hpp"
#include "LinkedCatDisplay.hpp"

using namespace geode::prelude;

class CatSettingsNode : public CCLayer {
public:
    static CatSettingsNode* create();

    void show();
    void hide();

    void setToCat(const CatStats& stats);

    void onCatApplyCallback(const std::function<void(const CatStats&)>& callback);

private:
    virtual bool init();

    void registerWithTouchDispatcher() override {
        cocos2d::CCTouchDispatcher::get()->addTargetedDelegate(this, -500, false);
    }

    ~CatSettingsNode() override{
        CCTouchDispatcher::get()->unregisterForcePrio(this);
    }

    virtual void keyBackClicked() override;

    void onBackClicked(CCObject*);

    bool isOpen;

    CatStats catToModify;

    std::function<void(const CatStats&)> callback = NULL;
    CCMenu* buttonsMenu;

    LinkedCatDisplay* catDisplay;

    bool doSwallow;
};
