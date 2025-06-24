#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CatSelectionPopup : public Popup<> {
    public:
        static CatSelectionPopup* create();

        virtual void show() override;

        void easeHorizontal(CCMoveBy* move);
    private:
        virtual bool setup() override;

        void onSelectAllClicked(CCObject*);
        void onDeselectAllClicked(CCObject*);

        virtual void onClose(CCObject*) override;

        ScrollLayer* catsScrollLayer;
};
