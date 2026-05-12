#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <types/CatStats.hpp>
#include <nodes/CatagoryAssetDisplay.hpp>

class CatVisualDisplay : public CCMenu {
public:
    static CatVisualDisplay* create();
    bool init() override;

    void updateVisuals(CatStats& stats);

    CatagoryAssetDisplay* getAssetForCategory(const std::string& catagoryName);

private:

    void updateIfChange(const std::string& catagoryName, CatagoryAssetDisplay* display, CatStats& oldS, CatStats& newS);

    std::optional<CatStats> current;

    std::vector<CatagoryAssetDisplay*> displays;
};
