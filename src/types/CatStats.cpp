#include "CatStats.hpp"

#include <utils/Utils.hpp>
#include <types/CatStatsSerializer.hpp>

#include <types/AREDLLevelDetailsSerializer.hpp>

const float CatStats::MIN_SIZE = .25f;
const float CatStats::MAX_SIZE = 2.0f;

CatStats CatStats::createDefault(GJGameLevel* relatedLevel){
    CatStats newStats;
    newStats.relatedLevel = relatedLevel;
    newStats.name = relatedLevel->m_levelName;
    newStats.catTypeID = 0;

    float randomSizeOffset = .2f;

    newStats.size = Utils::GetRandomFloat(CatStats::MIN_SIZE + randomSizeOffset, CatStats::MAX_SIZE - randomSizeOffset);
    
    return newStats;
}

Result<CatStats> CatStats::createFromJson(const matjson::Value& value){
    CatStats stats;
    GEODE_UNWRAP_INTO(stats.catTypeID, value["type"].asUInt());
    GEODE_UNWRAP_INTO(stats.name, value["name"].asString());
    GEODE_UNWRAP_INTO(stats.size, value["size"].asDouble());
    GEODE_UNWRAP_INTO(stats.hatID, value["hatID"].asUInt());
    GEODE_UNWRAP_INTO(stats.primaryColor, value["primaryColor"].as<ccColor4B>());
    GEODE_UNWRAP_INTO(stats.secondaryColor, value["secondaryColor"].as<ccColor4B>());

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

bool CatStats::isEmpty(){
    return relatedLevel == nullptr;
}

void CatStats::loadAREDLLevelData(){
    web::WebRequest req;
    auto task = req.get(fmt::format("https://api.aredl.net/v2/api/aredl/levels/{}", relatedLevel->m_levelID));

    task.listen([&](web::WebResponse* res) {
        if (res == nullptr || !res->ok()) return;
        
        auto jsonRes = res->json();
        if (!jsonRes.isOk()) return;
        auto json = jsonRes.unwrap();

        auto detailsRes = json.as<AREDLLevelDetails>();
        if (!detailsRes.isOk()) return;

        levelDetails = detailsRes.unwrap();

        if (onAREDLStatsRecieved != NULL) onAREDLStatsRecieved(this);
    });
}

void CatStats::setOnAREDLStatsRecievedCallback(std::function<void(CatStats*)> callback){
    onAREDLStatsRecieved = callback;
}

std::pair<std::string, std::string> CatStats::getSpritesPathsForCat(){
    return CatStats::getSpritesPathsForCat(this->catTypeID);
}

std::pair<std::string, std::string> CatStats::getSpritesPathsForCat(unsigned int typeID){
    return {fmt::format("cat-{}_0.png"_spr, typeID), fmt::format("cat-{}_1.png"_spr, typeID)};
}