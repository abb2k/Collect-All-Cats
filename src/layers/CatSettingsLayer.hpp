#pragma once

#include <Geode/Geode.hpp>
#include <types/CatStats.hpp>

using namespace geode::prelude;

class CatSettingsLayer : public CCLayer, public ColorPickerDelegate {
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

    CCMenu* topBarMenu;
    CCMenuItemSpriteExtra* addCatagory(const std::string& name, const std::string& resourceName, CCNode* visual);
    std::map<CCMenuItemSpriteExtra*, std::pair<std::string, std::string>> catagoriesMapped{};
    CCMenuItemSpriteExtra* selectedPage = nullptr;
    CCMenuItemSpriteExtra* defaultCatagory;
    void onCatagoryClicked(CCObject* sender);

    void onColorSkinsSwitch(CCObject* sender);
    CCMenuItemSpriteExtra* skinsSwitchBtn;
    CCMenuItemSpriteExtra* colorSwitchBtn;
    int currentEditingPage = 0;

    CCLabelBMFont* catagoryTitle;

    void colorValueChanged(ccColor3B newColor) override;
    CCMenu* colorOptionsContainer;
    CCControlColourPicker* colorPicker;
    TextInput* RInput;
    TextInput* GInput;
    TextInput* BInput;
    TextInput* AInput;
    void updateEditorColors();
    GLubyte getNumFromInput(const std::string& value);
    bool isColorModeDetail;
    void baseDetailSwap(CCObject*);
};
