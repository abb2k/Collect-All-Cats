#include "CatStats.hpp"

#include <utils/CatUtils.hpp>
#include <types/CatStatsSerializer.hpp>

#include <types/AREDLLevelDetailsSerializer.hpp>

#include <abb2k.rig/include/API.hpp>

const float CatStats::MIN_SIZE = .25f;
const float CatStats::MAX_SIZE = 2.0f;

CatStats CatStats::createDefault(GJGameLevel* relatedLevel){
    CatStats newStats;
    newStats.relatedLevel = relatedLevel;
    newStats.name = relatedLevel->m_levelName;
    newStats.setCatagoryAsset("cat", 0);

    float randomSizeOffset = .2f;

    newStats.size = CatUtils::GetRandomFloat(CatStats::MIN_SIZE + randomSizeOffset, CatStats::MAX_SIZE - randomSizeOffset);
    
    return newStats;
}

Result<CatStats> CatStats::createFromJson(const matjson::Value& value){
    CatStats stats;
    GEODE_UNWRAP_INTO(stats.name, value["name"].asString());
    GEODE_UNWRAP_INTO(stats.size, value["size"].asDouble());
    GEODE_UNWRAP_INTO(stats.customazationAssets, value["catagoryAssets"].as<std::map<std::string, CatagoryAssetInfo>>());

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
    return newStats;
}

bool CatStats::isEmpty(){
    return relatedLevel == nullptr;
}

void CatStats::loadAREDLLevelData(){
    if (levelDetails.has_value() && levelDetails.value().level_id == relatedLevel->m_levelID) return;

    log::info("Loading AREDL level data for level id {}", relatedLevel->m_levelID.value());
    
    web::WebRequest req;
    async::spawn(
        req.get(fmt::format("https://api.aredl.net/v2/api/aredl/levels/{}", relatedLevel->m_levelID)),
        [&](web::WebResponse value) {
            log::info("Received AREDL response for level id {} with status code {}", relatedLevel->m_levelID.value(), value.code());
            if (!value.ok()) return;

            log::info("Response body: {}", value.string().unwrapOr("Failed to get response body"));
        
            auto jsonRes = value.json();
            if (!jsonRes.isOk()) return;
            auto json = jsonRes.unwrap();

            log::info("Parsed JSON for AREDL level id {}: {}", relatedLevel->m_levelID.value(), json.dump());

            auto detailsRes = json.as<AREDLLevelDetails>();
            if (!detailsRes.isOk()) return;

            log::info("Parsed AREDLLevelDetails for level id {}: name = {}, position = {}, points = {}, publisher = {}", relatedLevel->m_levelID.value(), detailsRes.unwrap().name, detailsRes.unwrap().position, detailsRes.unwrap().points, detailsRes.unwrap().publisher.username);

            levelDetails = detailsRes.unwrap();

            if (onAREDLStatsRecieved != NULL) onAREDLStatsRecieved(this);
        }
    );
}

void CatStats::setOnAREDLStatsRecievedCallback(std::function<void(CatStats*)> callback){
    onAREDLStatsRecieved = callback;
}

CatagoryAssetInfo CatStats::getCatagoryAssetInfo(const std::string& catagoryResourceName){
    if (!customazationAssets.contains(catagoryResourceName))
        customazationAssets[catagoryResourceName] = CatagoryAssetInfo::createDefault(catagoryResourceName);

    return customazationAssets[catagoryResourceName];
}

void CatStats::setCatagoryAsset(const std::string& catagoryResourceName, const std::optional<unsigned int>& assetID){
    if (!customazationAssets.contains(catagoryResourceName))
        customazationAssets[catagoryResourceName] = CatagoryAssetInfo::createDefault(catagoryResourceName);

    customazationAssets[catagoryResourceName].assetID = assetID;
}

void CatStats::setCatagoryAssetPrimary(const std::string& catagoryResourceName, const ccColor4B& primary){
    if (!customazationAssets.contains(catagoryResourceName))
        customazationAssets[catagoryResourceName] = CatagoryAssetInfo::createDefault(catagoryResourceName);

    customazationAssets[catagoryResourceName].primary = primary;
}

void CatStats::setCatagoryAssetSecondary(const std::string& catagoryResourceName, const ccColor4B& secondary){
    if (!customazationAssets.contains(catagoryResourceName))
        customazationAssets[catagoryResourceName] = CatagoryAssetInfo::createDefault(catagoryResourceName);

    customazationAssets[catagoryResourceName].secondary = secondary;
}

Result<CatagoryAssetSprites> CatStats::getCatagoryAssetSprites(const std::string& catagoryResourceName, unsigned int itemID){
    bool isSecondaryFallback = true;
    bool isNoncolorFallback = true;

    CCSprite* primary = nullptr;
    std::shared_ptr<tinygltf::Model> model = nullptr;

    if (std::filesystem::exists(Mod::get()->getResourcesDir() / fmt::format("{}-{}_0.glb", catagoryResourceName, itemID))){
        auto modelRes = ModelLoader::loadModel(fmt::format("{}-{}_0.glb"_spr, catagoryResourceName, itemID));
        if (modelRes.isErr()){
            return Err("Failed to load model!");
        }
        
        model = modelRes.unwrap().model;

        if (!modelRes.unwrap().warning.empty())
            log::warn("{}", modelRes.unwrap().warning);
    }
    else if (std::filesystem::exists(Mod::get()->getResourcesDir() / fmt::format("{}-{}_0.png", catagoryResourceName, itemID))){
        primary = CCSprite::create(fmt::format("{}-{}_0.png"_spr, catagoryResourceName, itemID).c_str());
    }
    else{
        return Err("No resource for catagory {} of ID {} exists!", catagoryResourceName, itemID);
    }

    if (std::filesystem::exists(Mod::get()->getResourcesDir() / fmt::format("{}-{}_1.png", catagoryResourceName, itemID))){
        isSecondaryFallback = false;
    }

    if (std::filesystem::exists(Mod::get()->getResourcesDir() / fmt::format("{}-{}_2.png", catagoryResourceName, itemID))){
        isNoncolorFallback = false;
    }

    return Ok(CatagoryAssetSprites{
        .primary = primary,
        .secondary = isSecondaryFallback ? std::nullopt : std::make_optional(std::move(CCSprite::create(fmt::format("{}-{}_1.png"_spr, catagoryResourceName, itemID).c_str()))),
        .noncolor = isNoncolorFallback ? std::nullopt : std::make_optional(std::move(CCSprite::create(fmt::format("{}-{}_2.png"_spr, catagoryResourceName, itemID).c_str()))),
        .model = model
    });
}

std::string CatStats::getRealName(){
    return getLevel()->m_levelName == "" ? (getLevelDetails() != nullptr ? getLevelDetails()->name : "Unknown") : getLevel()->m_levelName;
}

void CatStats::setLevelDetails(const AREDLLevelDetails& details){
    levelDetails = details;
}