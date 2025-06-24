#pragma once

#include <Geode/Geode.hpp>
#include "../../layers/CatsLayer.hpp"

using namespace geode::prelude;

class CatSelectionPopup : public Popup<> {
    public:
        static CatSelectionPopup* create();

    private:
        virtual bool setup() override;

        void onSelectAllClicked(CCObject*);
        void onDeselectAllClicked(CCObject*);

        ScrollLayer* catsScrollLayer;
};
