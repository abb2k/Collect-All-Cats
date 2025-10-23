#pragma once

#include <string>
#include <vector>

struct AREDLUser{
    std::string id;
    std::string username;
    std::string global_name;
};

struct AREDLVerification{
    std::string id;
    AREDLUser submitted_by;
    bool mobile;
    std::string video_url;
    std::string created_at;
};

struct AREDLLevelDetails{
    std::string id;
    int position;
    std::string name;
    float points;
    bool legacy;
    int level_id;
    bool two_player;
    std::vector<std::string> tags;
    std::string description;
    int songID;
    int edel_enjoyment;
    bool is_edel_pending;
    int gddl_tier;
    std::string nlw_tier;
    AREDLUser publisher;
    std::vector<AREDLVerification> verifications;
};
