#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

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

    private:
        GJGameLevel* relatedLevel;
        CatStats() {}
};
