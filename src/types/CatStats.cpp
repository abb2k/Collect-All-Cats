#include "CatStats.hpp"

#include <utils/Utils.hpp>
#include <types/CatStatsSerializer.hpp>

const float CatStats::MIN_SIZE = .5f;
const float CatStats::MAX_SIZE = 2.0f;

CatStats CatStats::createDefault(GJGameLevel* relatedLevel){
    CatStats newStats;
    newStats.relatedLevel = relatedLevel;
    newStats.name = relatedLevel->m_levelName;
    newStats.catTypeID = Utils::GetRandomInt(0, 10);
    
    return newStats;
}

Result<CatStats> CatStats::createFromJson(const matjson::Value& value){
    CatStats stats;
    GEODE_UNWRAP_INTO(stats.catTypeID, value["type"].asUInt());
    GEODE_UNWRAP_INTO(stats.name, value["name"].asString());
    GEODE_UNWRAP_INTO(stats.size, value["size"].asDouble());

    return Ok(stats);
}

Result<CatStats> CatStats::createFromJsonWithLevel(const matjson::Value& json, GJGameLevel* level){
    GEODE_UNWRAP_INTO(auto stats, json.as<CatStats>());
    stats.relatedLevel = level;

    return Ok(stats);
}

GJGameLevel* CatStats::getLevel() { return relatedLevel; }

CatStats CatStats::createEmpty(){
    CatStats newStats;
    newStats.relatedLevel = nullptr;
    newStats.catTypeID = -1;
    return newStats;
}