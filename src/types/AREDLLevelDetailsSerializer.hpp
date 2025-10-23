#pragma once

#include <Geode/Geode.hpp>
#include <types/AREDLLevelDetails.hpp>

using namespace geode::prelude;

template <>
struct matjson::Serialize<AREDLUser> {
    static Result<AREDLUser> fromJson(const matjson::Value& value){
        AREDLUser user;
        GEODE_UNWRAP_INTO(user.id, value["id"].asString());
        GEODE_UNWRAP_INTO(user.username, value["username"].asString());
        GEODE_UNWRAP_INTO(user.global_name, value["global_name"].asString());

        return Ok(user);
    }

    static matjson::Value toJson(const AREDLUser& value){
        matjson::Value obj = matjson::makeObject({
        });
        return obj;
    }
};

template <>
struct matjson::Serialize<AREDLVerification> {
    static Result<AREDLVerification> fromJson(const matjson::Value& value){
        AREDLVerification verification;
        GEODE_UNWRAP_INTO(verification.id, value["id"].asString());
        GEODE_UNWRAP_INTO(verification.submitted_by, value["submitted_by"].as<AREDLUser>());
        GEODE_UNWRAP_INTO(verification.mobile, value["mobile"].asBool());
        GEODE_UNWRAP_INTO(verification.video_url, value["video_url"].asString());
        GEODE_UNWRAP_INTO(verification.created_at, value["created_at"].asString());

        return Ok(verification);
    }

    static matjson::Value toJson(const AREDLVerification& value){
        matjson::Value obj = matjson::makeObject({
        });
        return obj;
    }
};

template <>
struct matjson::Serialize<AREDLLevelDetails> {
    static Result<AREDLLevelDetails> fromJson(const matjson::Value& value){
        AREDLLevelDetails details;
        GEODE_UNWRAP_INTO(details.id, value["id"].asString());
        GEODE_UNWRAP_INTO(details.position, value["position"].asInt());
        GEODE_UNWRAP_INTO(details.name, value["name"].asString());
        GEODE_UNWRAP_INTO(details.points, value["points"].asDouble());
        GEODE_UNWRAP_INTO(details.legacy, value["legacy"].asBool());
        GEODE_UNWRAP_INTO(details.level_id, value["level_id"].asInt());
        GEODE_UNWRAP_INTO(details.two_player, value["two_player"].asBool());
        GEODE_UNWRAP_INTO(details.tags, value["tags"].as<std::vector<std::string>>());
        GEODE_UNWRAP_INTO(details.description, value["description"].asString());
        GEODE_UNWRAP_INTO(details.songID, value["song"].asInt());
        GEODE_UNWRAP_INTO(details.edel_enjoyment, value["edel_enjoyment"].asInt());
        GEODE_UNWRAP_INTO(details.is_edel_pending, value["is_edel_pending"].asBool());
        GEODE_UNWRAP_INTO(details.gddl_tier, value["gddl_tier"].asInt());
        GEODE_UNWRAP_INTO(details.nlw_tier, value["nlw_tier"].asString());
        GEODE_UNWRAP_INTO(details.publisher, value["publisher"].as<AREDLUser>());
        GEODE_UNWRAP_INTO(details.verifications, value["verifications"].as<std::vector<AREDLVerification>>());

        return Ok(details);
    }

    static matjson::Value toJson(const AREDLLevelDetails& value){
        matjson::Value obj = matjson::makeObject({
        });
        return obj;
    }
};