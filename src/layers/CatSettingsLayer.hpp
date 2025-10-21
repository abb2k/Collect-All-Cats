#pragma once

#include <Geode/Geode.hpp>
#include <types/CatStats.hpp>

using namespace geode::prelude;

class CatSettingsLayer : public CCLayer {
public:
    static CatSettingsLayer* create();

    void showWithCat(CatStats& stats);

    void show();
    void hide();

    void setToCat(CatStats& stats);

private:
    virtual bool init() override;

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
    CCMenuItemSpriteExtra* nameResetBtn;
    void nameReset(CCObject*);

    bool isOpen;

    CatStats catToModify = CatStats::createEmpty();

    CCMenu* buttonsMenu;

    bool doSwallow;

    void updateLivingCat();

};
