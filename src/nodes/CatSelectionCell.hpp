#pragma once

#include <Geode/Geode.hpp>
#include <types/CatStats.hpp>
#include <layers/CatsLayer.hpp>
#include <nodes/SimpleToggler.hpp>

using namespace geode::prelude;

class CatSelectionCell : public CCMenu {
public:
    static CatSelectionCell* create(GJGameLevel* level);

    void togglePlaced(bool placed, bool changeToggleSprite);

private:
    virtual bool init(GJGameLevel* level);

    CatStats myCatStats = CatStats::createEmpty();
    SimpleToggler* selectedToggle;

    void onCatSettingsClicked(CCObject*);

    CatsLayer* catsLayer;

    void onStatsChanged(const CatStats& newStats);

    CCLabelBMFont* nameLabel;
    CCLabelBMFont* levelNameLabel;
};
