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
        GEODE_UNWRAP_INTO(verification.hide_video, value["hide_video"].asBool());
        GEODE_UNWRAP_INTO(verification.achived_at, value["achived_at"].asString());
        GEODE_UNWRAP_INTO(verification.created_at, value["created_at"].asString());
        GEODE_UNWRAP_INTO(verification.updated_at, value["updated_at"].asString());

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
        log::info("1");
        GEODE_UNWRAP_INTO(details.id, value["id"].asString());
        log::info("2");
        GEODE_UNWRAP_INTO(details.position, value["position"].asInt());
        log::info("3");
        GEODE_UNWRAP_INTO(details.name, value["name"].asString());
        log::info("4");
        GEODE_UNWRAP_INTO(details.points, value["points"].asDouble());
        log::info("5");
        GEODE_UNWRAP_INTO(details.legacy, value["legacy"].asBool());
        log::info("6");
        GEODE_UNWRAP_INTO(details.level_id, value["level_id"].asInt());
        log::info("7");
        GEODE_UNWRAP_INTO(details.two_player, value["two_player"].asBool());
        log::info("8");
        GEODE_UNWRAP_INTO(details.tags, value["tags"].as<std::vector<std::string>>());
        log::info("9");
        GEODE_UNWRAP_INTO(details.description, value["description"].asString());
        log::info("10");
        if (!value["song"].isNull()){
            GEODE_UNWRAP_INTO(details.songID, value["song"].asInt());
        }
        log::info("11");
        GEODE_UNWRAP_INTO(details.edel_enjoyment, value["edel_enjoyment"].asDouble());
        log::info("12");
        GEODE_UNWRAP_INTO(details.is_edel_pending, value["is_edel_pending"].asBool());
        log::info("13");
        GEODE_UNWRAP_INTO(details.gddl_tier, value["gddl_tier"].asDouble());
        log::info("14");
        GEODE_UNWRAP_INTO(details.nlw_tier, value["nlw_tier"].asString());
        log::info("15");
        GEODE_UNWRAP_INTO(details.publisher, value["publisher"].as<AREDLUser>());
        log::info("16");
        GEODE_UNWRAP_INTO(details.verifications, value["verifications"].as<std::vector<AREDLVerification>>());
        log::info("gg :3");

        return Ok(details);
    }

    static matjson::Value toJson(const AREDLLevelDetails& value){
        matjson::Value obj = matjson::makeObject({
        });
        return obj;
    }
};