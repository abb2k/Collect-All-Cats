#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CatSelectionPopup : public Popup<> {
    public:
        static CatSelectionPopup* create();

        virtual void show() override;

        void fadeTo(GLubyte opacity, float time);
    private:
        virtual bool setup() override;

        void onSelectAllClicked(CCObject*);
        void onDeselectAllClicked(CCObject*);

        virtual void onClose(CCObject*) override;

        ScrollLayer* catsScrollLayer;

        ButtonSprite* selAllBtnSprite;
        ButtonSprite* deselAllBtnSprite;
};
