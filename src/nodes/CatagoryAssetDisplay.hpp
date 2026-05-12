#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <types/CatStats.hpp>

#include <abb2k.rig/include/API.hpp>

class CatagoryAssetDisplay : public CCMenu {
public:
    static CatagoryAssetDisplay* create();
    bool init() override;

    void setAsset(const std::string& catagoryName, const std::optional<unsigned int>& itemID);
    void setAsset(const CatagoryAssetInfo& catagoryName);
    void setPrimaryColor(const ccColor4B& primary);
    void setSecondaryColor(const ccColor4B& secondary);

    void setAssetUpdatedCallback(const std::function<void(CatagoryAssetDisplay*)>& onAssetUpdated);

    CosmeticMetadata getMetadata() const { return currentMetadata; }

    void setCategory(const std::string& catagoryName);
    std::string getCategory() const { return currentAsset.catagoryResoueceName; }

    std::optional<CosmeticBoneOffset> isBoneChild(const std::string& categoryName);

    CCNode* getSkeletonBone(const std::string& boneName);

    void applyOffset(const CosmeticOffset& off);

    SkeletonPlayer* getModel() const { return m_model; }

private:

    CatagoryAssetInfo currentAsset;

    CosmeticMetadata currentMetadata;

    CCSprite* m_primarySprite = nullptr;
    CCSprite* m_secondarySprite = nullptr;
    CCSprite* m_noncolorSprite = nullptr;
    SkeletonPlayer* m_model = nullptr;

    void initSprite(CCSprite* spr, const char* id);

    std::function<void(CatagoryAssetDisplay*)> onAssetUpdated = NULL;
};
