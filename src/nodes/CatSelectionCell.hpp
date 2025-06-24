#pragma once

#include <Geode/Geode.hpp>
#include "Cat.hpp"
#include "../layers/CatsLayer.hpp"

using namespace geode::prelude;

class CatSelectionCell : public CCScale9Sprite {
public:
    static CatSelectionCell* create(GJGameLevel* level);

    void togglePlaced(bool placed, bool changeToggleSprite);

private:
    virtual bool init(GJGameLevel* level);

    CatStats myCatStats;
    CCMenuItemToggler* selectedToggle;

    void onCatSettingsClicked(CCObject*);
    void onSelectedToggled(CCObject*);

    CatsLayer* catsLayer;
};
