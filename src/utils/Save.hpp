#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include "../nodes/Cat.hpp"

template <>
struct matjson::Serialize<CatStats> {
    static Result<CatStats> fromJson(const matjson::Value& value) {

        CatStats stats;
        GEODE_UNWRAP_INTO(stats.catTypeID, value["type"].asUInt());
        GEODE_UNWRAP_INTO(stats.name, value["name"].asString());
        GEODE_UNWRAP_INTO(stats.size, value["size"].asDouble());

        return Ok(stats);
    }

    static matjson::Value toJson(const CatStats& value) {
        matjson::Value obj = matjson::makeObject({
            { "type", value.catTypeID },
            { "name", value.name },
            { "size", value.size }
        });
        return obj;
    }
};

class Save{
    private:
        static std::filesystem::path savesPath;

        static std::string getCatSaveFileName(GJGameLevel* relatedLevel);

    public:
        static Result<> saveCat(Cat* cat);
        static Result<> saveCat(const CatStats& stats);

        static Result<CatStats> loadCat(GJGameLevel* relatedLevel);

        static std::vector<int> getPlacedCats();
        static void setPlacedCats(const std::vector<int>& catIDs);
        static void addPlacedCat(int catID);
        static void removePlacedCats(int catID);
};