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

    auto catDisplay = CatagoryAssetDisplay::create();
    catDisplay->setPosition(this->getContentSize() / 2);
    catDisplay->setCategory("cat");
    this->addChild(catDisplay);

    auto hatDisplay = CatagoryAssetDisplay::create();
    hatDisplay->setPosition(this->getContentSize() / 2);
    hatDisplay->setCategory("hat");
    this->addChild(hatDisplay);

    auto glassesDisplay = CatagoryAssetDisplay::create();
    glassesDisplay->setPosition(this->getContentSize() / 2);
    glassesDisplay->setCategory("glasses");
    this->addChild(glassesDisplay);

    auto neckDisplay = CatagoryAssetDisplay::create();
    neckDisplay->setPosition(this->getContentSize() / 2);
    neckDisplay->setCategory("neck");
    this->addChild(neckDisplay);

    auto shoeDisplay = CatagoryAssetDisplay::create();
    shoeDisplay->setPosition(this->getContentSize() / 2);
    shoeDisplay->setCategory("shoe");
    this->addChild(shoeDisplay);

    displays = {catDisplay, hatDisplay, glassesDisplay, neckDisplay, shoeDisplay};

    for (const auto& display : displays){
        display->setAssetUpdatedCallback([&](CatagoryAssetDisplay* display){
            display->setPosition(this->getContentSize() / 2);
            display->setRotation(0);
            display->setScale(.7f);

            bool didFindParent = false;

            for (const auto& otherDisplay : displays){
                auto boneCheckRes = otherDisplay->isBoneChild(display->getCategory());
                if (!boneCheckRes.has_value()) continue;
                auto offData = boneCheckRes.value();

                auto bone = otherDisplay->getSkeletonBone(offData.boneName);
                if (bone == nullptr) continue;

                didFindParent = true;

                display->retain();
                display->removeFromParent();
                bone->addChild(display);
                display->release();
                display->setPosition({0, 0});
                display->applyOffset(offData.offset);
                
                break;
            }

            if (!didFindParent){
                display->retain();
                display->removeFromParent();
                this->addChild(display);
                display->release();
            }
            
            display->applyOffset(display->getMetadata().personalOffset);
        });
    }

    return true;
}

void CatVisualDisplay::updateVisuals(CatStats& stats){
    CatStats* cur = current.has_value() ? &current.value() : nullptr;

    for (const auto& display : displays)
    {
        if (cur == nullptr){
            display->setAsset(stats.getCatagoryAssetInfo(display->getCategory()));
        }
        else{
            updateIfChange(display->getCategory(), display, *cur, stats);
        }
    }

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

CatagoryAssetDisplay* CatVisualDisplay::getAssetForCategory(const std::string& catagoryName){
    for (const auto& display : displays)
        if (display->getCategory() == catagoryName)
            return display;
    return nullptr;
}