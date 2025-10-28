#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <types/CatStats.hpp>

class CatVisualDisplay : public CCMenu {
public:
    static CatVisualDisplay* create();

    void updateVisuals(CatStats& stats);

    void updateCoreSprites(unsigned int typeID);

    void updateCorePrimary(ccColor4B primary);
    void updateCoreSecondary(ccColor4B secondary);

private:
    virtual bool init();

    std::optional<CatStats> current = std::nullopt;
};
