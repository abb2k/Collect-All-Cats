#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <types/CatStats.hpp>

class CatagoryAssetDisplay : public CCMenu {
public:
    static CatagoryAssetDisplay* create();
    bool init() override;

    void setAsset(const std::string& catagoryName, const std::optional<unsigned int>& itemID);
    void setAsset(const CatagoryAssetInfo& catagoryName);
    void setPrimaryColor(const ccColor4B& primary);
    void setSecondaryColor(const ccColor4B& secondary);

private:

    CatagoryAssetInfo currentAsset;

    CCSprite* m_primarySprite = nullptr;
    CCSprite* m_secondarySprite = nullptr;
    CCSprite* m_noncolorSprite = nullptr;

    void initSprite(CCSprite* spr, const char* id);
};
