#pragma once

#include <Geode/Geode.hpp>
#include "../types/CatStats.hpp"
#include "../layers/CatsLayer.hpp"

using namespace geode::prelude;

class CatSelectionCell : public CCScale9Sprite {
public:
    static CatSelectionCell* create(GJGameLevel* level);

    void togglePlaced(bool placed, bool changeToggleSprite);

private:
    virtual bool init(GJGameLevel* level);

    CatStats myCatStats = CatStats::createEmpty();
    CCMenuItemToggler* selectedToggle;

    void onCatSettingsClicked(CCObject*);
    void onSelectedToggled(CCObject*);

    CatsLayer* catsLayer;

    void onStatsChanged(const CatStats& newStats);

    CCLabelBMFont* nameLabel;
    CCLabelBMFont* levelNameLabel;
};
