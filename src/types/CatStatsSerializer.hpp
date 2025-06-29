#pragma once

#include <Geode/Geode.hpp>
#include "CatStats.hpp"

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
            { "size", value.size }
        });
        return obj;
    }
};