#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <types/AREDLLevelDetails.hpp>

struct CatStats{
    public:
        std::string name;
        unsigned int catTypeID;
        float size = 1;

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

    private:
        GJGameLevel* relatedLevel;
        std::optional<AREDLLevelDetails> levelDetails = std::nullopt;
        CatStats() {}

        std::function<void(CatStats*)> onAREDLStatsRecieved = NULL;
};
