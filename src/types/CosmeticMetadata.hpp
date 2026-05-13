#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define RENDER_TYPE_MODEL "model"
#define RENDER_TYPE_TEXTURE "texture"

struct CosmeticOffset{
    CCPoint posOffset;
    CCPoint scaleOffset;
    CCPoint rotationOffset;

    static CosmeticOffset none(){
        return CosmeticOffset{};
    }
};

struct CosmeticBoneOffset{
    std::string boneName;
    CosmeticOffset offset;
};

struct CosmeticMetadata{
    std::string renderType; //model/texture
    std::map<std::string, CosmeticBoneOffset> boneOffsets; // category - {boneName, offset from bone}
    CosmeticOffset personalOffset;
    CCSize modelSize;
    int cost;
};