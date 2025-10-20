#pragma once

#include <Geode/Geode.hpp>
#include <types/CatStats.hpp>

using namespace geode::prelude;

class LinkedCatDisplay : public CCMenu {
public:
    static LinkedCatDisplay* create(CatStats* catStats);

    void setCat(const CatStats& catStats);

    void setNameAboveVisible(bool b);

private:
    virtual bool init(CatStats* catStats);

    void update(float dt);

    Result<CatStats> myStats = Err("");
    CCSprite* kittyColonThreeSprite;

    CCLabelBMFont* nameLabel;
};
