#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <types/CatStats.hpp>

class CatVisualDisplay : public CCMenu {
public:
    static CatVisualDisplay* create();
    bool init() override;

    void updateVisuals(const CatStats& stats);

private:
    void updateCoreSprites(unsigned int typeID);
    void updateCorePrimary(ccColor4B primary);
    void updateCoreSecondary(ccColor4B secondary);

    CCSprite* m_primarySprite = nullptr;
    CCSprite* m_secondarySprite = nullptr;
    CCSprite* m_noncolorSprite = nullptr;

    std::optional<CatStats> current;

    void initSprite(CCSprite* spr, const char* id);
};
