#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SimpleToggler : public CCMenu {
    public:
        static SimpleToggler* create(CCNode* offSprite, CCNode* onSprite, float scale = 1, bool startState = false);
        static SimpleToggler* createWithDefaults(float scale = 1, bool startState = false);

        void setCallback(const std::function<void(bool)>& callback);

        void setOpacity(GLubyte opacity) override;

        void toggle(bool state, bool withCallback = false);

        const bool getState() { return state; }
        
    private:
        bool init(CCNode* offSprite, CCNode* onSprite, float scale, bool startState);

        void onToggledOn(CCObject* sender);
        void onToggledOff(CCObject* sender);

        std::function<void(bool)> callback = NULL;
        CCNode* offSprite;
        CCNode* onSprite;

        CCMenuItemSpriteExtra* offBtn;
        CCMenuItemSpriteExtra* onBtn;

        bool state;
};