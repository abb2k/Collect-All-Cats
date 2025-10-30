#pragma once

#include <Geode/Geode.hpp>
#include <types/CatStats.hpp>

using namespace geode::prelude;

template <>
struct matjson::Serialize<CatagoryAssetInfo> {
    static Result<CatagoryAssetInfo> fromJson(const matjson::Value& value){
        CatagoryAssetInfo assets;

        GEODE_UNWRAP_INTO(assets.catagoryResoueceName, value["resourceName"].asString());

        int assetIDRaw = 0;
        GEODE_UNWRAP_INTO(assetIDRaw, value["assetID"].asInt());
        assets.assetID = assetIDRaw == -1 ? std::nullopt : std::make_optional(assetIDRaw);

        GEODE_UNWRAP_INTO(assets.primary, value["primary"].as<ccColor4B>());
        GEODE_UNWRAP_INTO(assets.secondary, value["secondary"].as<ccColor4B>());

        return Ok(assets);
    }

    static matjson::Value toJson(const CatagoryAssetInfo& value){
        matjson::Value obj = matjson::makeObject({
            { "resourceName", value.catagoryResoueceName },
            { "assetID", value.assetID == std::nullopt ? -1 : value.assetID.value() },
            { "primary", value.primary},
            { "secondary", value.secondary}
        });
        return obj;
    }
};
template <>
struct matjson::Serialize<CatStats> {
    static Result<CatStats> fromJson(const matjson::Value& value){
        return CatStats::createFromJson(value);
    }

    static matjson::Value toJson(const CatStats& value){

        matjson::Value obj = matjson::makeObject({
            { "name", value.name },
            { "size", value.size },
            { "catagoryAssets", value.customazationAssets}
        });
        return obj;
    }
};