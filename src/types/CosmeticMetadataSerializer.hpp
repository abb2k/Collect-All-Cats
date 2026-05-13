#pragma once

#include <Geode/Geode.hpp>
#include <types/CosmeticMetadata.hpp>

using namespace geode::prelude;

template <>
struct matjson::Serialize<CCPoint> {
    static Result<CCPoint> fromJson(const matjson::Value& value){
        CCPoint point;
        GEODE_UNWRAP_INTO(point.x, value["x"].asDouble());
        GEODE_UNWRAP_INTO(point.y, value["y"].asDouble());

        return Ok(point);
    }

    static matjson::Value toJson(const CCPoint& value){
        matjson::Value obj = matjson::makeObject({
            { "x", value.x },
            { "y", value.y }
        });
        return obj;
    }
};


template <>
struct matjson::Serialize<CCSize> {
    static Result<CCSize> fromJson(const matjson::Value& value){
        CCSize size;
        GEODE_UNWRAP_INTO(size.width, value["width"].asDouble());
        GEODE_UNWRAP_INTO(size.height, value["height"].asDouble());

        return Ok(size);
    }

    static matjson::Value toJson(const CCSize& value){
        matjson::Value obj = matjson::makeObject({
            { "width", value.width },
            { "height", value.height }
        });
        return obj;
    }
};

template <>
struct matjson::Serialize<CosmeticOffset> {
    static Result<CosmeticOffset> fromJson(const matjson::Value& value){
        CosmeticOffset off;
        GEODE_UNWRAP_INTO(off.posOffset, value["posOffset"].as<CCPoint>());
        GEODE_UNWRAP_INTO(off.scaleOffset, value["scaleOffset"].as<CCPoint>());
        GEODE_UNWRAP_INTO(off.rotationOffset, value["rotationOffset"].as<CCPoint>());

        return Ok(off);
    }

    static matjson::Value toJson(const CosmeticOffset& value){
        matjson::Value obj = matjson::makeObject({
            { "posOffset", value.posOffset },
            { "scaleOffset", value.scaleOffset },
            { "rotationOffset", value.rotationOffset }
        });
        return obj;
    }
};

template <>
struct matjson::Serialize<CosmeticBoneOffset> {
    static Result<CosmeticBoneOffset> fromJson(const matjson::Value& value){
        CosmeticBoneOffset boneOff;
        GEODE_UNWRAP_INTO(boneOff.boneName, value["boneName"].asString());
        GEODE_UNWRAP_INTO(boneOff.offset, value["offset"].as<CosmeticOffset>());

        return Ok(boneOff);
    }

    static matjson::Value toJson(const CosmeticBoneOffset& value){
        matjson::Value obj = matjson::makeObject({
            { "boneName", value.boneName },
            { "offset", value.offset }
        });
        return obj;
    }
};

template <>
struct matjson::Serialize<CosmeticMetadata> {
    static Result<CosmeticMetadata> fromJson(const matjson::Value& value){
        CosmeticMetadata meta;
        GEODE_UNWRAP_INTO(meta.renderType, value["renderType"].asString());
        
        auto boneOffRes = value["boneOffsets"].as<std::map<std::string, CosmeticBoneOffset>>();
        if (boneOffRes.isOk()){
            meta.boneOffsets = boneOffRes.unwrap();
        }
        else{
            meta.boneOffsets = {};
        }

        GEODE_UNWRAP_INTO(meta.personalOffset, value["personalOffset"].as<CosmeticOffset>());

        if (meta.renderType == RENDER_TYPE_MODEL){
            GEODE_UNWRAP_INTO(meta.modelSize, value["modelSize"].as<CCSize>());
        }
        else{
            auto mSizeRes = value["modelSize"].as<CCSize>();
            if (mSizeRes.isOk()) meta.modelSize = mSizeRes.unwrap();
            else meta.modelSize = CCSize{0, 0};
        }

        auto costRes = value["cost"].asInt();

        if (costRes.isOk()){
            meta.cost = costRes.unwrap();
            if (meta.cost < 0) meta.cost = 0;
        }
        else
            meta.cost = 0;

        return Ok(meta);
    }

    static matjson::Value toJson(const CosmeticMetadata& value){
        matjson::Value obj = matjson::makeObject({
            { "renderType", value.renderType },
            { "boneOffsets", value.boneOffsets },
            { "personalOffset", value.personalOffset },
            { "modelSize", value.modelSize }
        });
        return obj;
    }
};