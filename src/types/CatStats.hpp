#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <types/AREDLLevelDetails.hpp>

struct CatagoryAssetInfo{
    std::string catagoryResoueceName;
    std::optional<unsigned int> assetID;
    ccColor4B primary;
    ccColor4B secondary;

    static CatagoryAssetInfo createDefault(const std::string& catagoryResoueceName, unsigned int assetID = -1){
        CatagoryAssetInfo toReturn{};
        toReturn.catagoryResoueceName = catagoryResoueceName;
        toReturn.assetID = assetID == -1 ? std::nullopt : std::make_optional(assetID);
        toReturn.primary = {255, 255, 255, 255};
        toReturn.secondary = {255, 255, 255, 255};

        return toReturn;
    }
};

struct CatagoryAssetSprites{
    CCSprite* primary;
    std::optional<CCSprite*> secondary;
    std::optional<CCSprite*> noncolor;
};

struct CatStats{
    public:
        std::string name;
        float size = 1;
        std::map<std::string, CatagoryAssetInfo> customazationAssets{};

        static const float MIN_SIZE;
        static const float MAX_SIZE;

        static CatStats createDefault(GJGameLevel* relatedLevel);

        static Result<CatStats> createFromJson(const matjson::Value& value);

        static Result<CatStats> createFromJsonWithLevel(const matjson::Value& json, GJGameLevel* level);
        static CatStats createEmpty();

        GJGameLevel* getLevel();

        bool isEmpty();

        const AREDLLevelDetails* getLevelDetails(){
            if (levelDetails.has_value()) return &levelDetails.value();
            return nullptr;
        }

        void setOnAREDLStatsRecievedCallback(std::function<void(CatStats*)> callback);
        
        void loadAREDLLevelData();

        CatagoryAssetInfo getCatagoryAssetInfo(const std::string& catagoryResourceName);
        void setCatagoryAsset(const std::string& catagoryResourceName, const std::optional<unsigned int>& assetID);
        void setCatagoryAssetPrimary(const std::string& catagoryResourceName, const ccColor4B& primary);
        void setCatagoryAssetSecondary(const std::string& catagoryResourceName, const ccColor4B& secondary);

        static Result<CatagoryAssetSprites> getCatagoryAssetSprites(const std::string& catagoryResourceName, unsigned int itemID);

    private:
        GJGameLevel* relatedLevel;
        std::optional<AREDLLevelDetails> levelDetails = std::nullopt;
        CatStats() {}

        std::function<void(CatStats*)> onAREDLStatsRecieved = NULL;
};