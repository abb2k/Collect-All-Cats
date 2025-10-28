#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <types/AREDLLevelDetails.hpp>

struct CatStats{
    public:
        std::string name;
        unsigned int catTypeID;
        float size = 1;
        unsigned int hatID;
        ccColor4B primaryColor = {255, 255, 255, 255};
        ccColor4B secondaryColor = {255, 255, 255, 255};

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

        std::pair<std::string, std::string> getSpritesPathsForCat();
        static std::pair<std::string, std::string> getSpritesPathsForCat(unsigned int typeID);

    private:
        GJGameLevel* relatedLevel;
        std::optional<AREDLLevelDetails> levelDetails = std::nullopt;
        CatStats() {}

        std::function<void(CatStats*)> onAREDLStatsRecieved = NULL;
};
