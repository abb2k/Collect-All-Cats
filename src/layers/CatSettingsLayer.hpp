#pragma once

#include <Geode/Geode.hpp>
#include "../nodes/LinkedCatDisplay.hpp"

using namespace geode::prelude;

class CatSettingsLayer : public CCLayer {
public:
    static CatSettingsLayer* create();

    void show();
    void hide();

    void setToCat(const CatStats& stats);

    void onCatApplyCallback(const std::function<void(const CatStats&)>& callback);

private:
    virtual bool init();

    void registerWithTouchDispatcher() override {
        cocos2d::CCTouchDispatcher::get()->addTargetedDelegate(this, -500, false);
    }

    ~CatSettingsLayer() override{
        CCTouchDispatcher::get()->unregisterForcePrio(this);
    }

    virtual void keyBackClicked() override;

    void onBackClicked(CCObject*);
    void onSizeValueChanged(CCObject*);
    Slider* sizeScroll;
    TextInput* sizeInputField;

    TextInput* nameInputField;

    bool isOpen;

    CatStats catToModify = CatStats::createEmpty();

    std::function<void(const CatStats&)> callback = NULL;
    CCMenu* buttonsMenu;

    LinkedCatDisplay* catDisplay;

    bool doSwallow;

    void applyChanges(CCObject*);
};
