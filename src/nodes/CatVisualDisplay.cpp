#include "CatVisualDisplay.hpp"

CatVisualDisplay* CatVisualDisplay::create() {
    auto ret = new CatVisualDisplay();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CatVisualDisplay::init() {
    if (!CCMenu::init()) return false;

    this->setPosition({0, 0});
    this->setContentSize({150, 150});
    this->ignoreAnchorPointForPosition(false);

    catDisplay = CatagoryAssetDisplay::create();
    catDisplay->setScale(.7f);
    catDisplay->setPosition(this->getContentSize() / 2);
    this->addChild(catDisplay);

    return true;
}

void CatVisualDisplay::updateVisuals(CatStats& stats){
    CatStats* cur = current.has_value() ? &current.value() : nullptr;
    
    if (cur == nullptr){
        catDisplay->setAsset(stats.getCatagoryAssetInfo("cat"));
    }
    else{
        updateIfChange("cat", catDisplay, *cur, stats);
    }

    catDisplay->setPosition(this->getContentSize() / 2);

    current = stats;
}

void CatVisualDisplay::updateIfChange(const std::string& catagoryName, CatagoryAssetDisplay* display, CatStats& oldS, CatStats& newS){
    auto currentCatAssetInfo = oldS.getCatagoryAssetInfo(catagoryName);
    auto newCatAssetInfo = newS.getCatagoryAssetInfo(catagoryName);

    if (currentCatAssetInfo.assetID != newCatAssetInfo.assetID)
        display->setAsset(catagoryName, newCatAssetInfo.assetID);

    if (currentCatAssetInfo.primary != newCatAssetInfo.primary)
        display->setPrimaryColor(newCatAssetInfo.primary);
    if (currentCatAssetInfo.secondary != newCatAssetInfo.secondary)
        display->setSecondaryColor(newCatAssetInfo.secondary);
}