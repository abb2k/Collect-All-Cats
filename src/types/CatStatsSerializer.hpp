#pragma once

#include <Geode/Geode.hpp>
#include <types/CatStats.hpp>

using namespace geode::prelude;

template <>
struct matjson::Serialize<CatStats> {
    static Result<CatStats> fromJson(const matjson::Value& value){
        return CatStats::createFromJson(value);
    }

    static matjson::Value toJson(const CatStats& value){
        matjson::Value obj = matjson::makeObject({
            { "type", value.catTypeID },
            { "name", value.name },
            { "size", value.size },
            { "hatID", value.hatID },
            { "primaryColor", value.primaryColor},
            { "secondaryColor", value.secondaryColor},
        });
        return obj;
    }
};