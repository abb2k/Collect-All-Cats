#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CosmeticOffset{
    CCPoint posOffset;
    CCPoint scaleOffset;
    CCPoint rotationOffset;
};

struct CosmeticBoneOffset{
    std::string boneName;
    CosmeticOffset offset;
};

struct CosmeticMetadata{
    std::string renderType; //Model/Texture
    std::map<std::string, CosmeticBoneOffset> boneOffsets; // category - {boneName, offset from bone}
    CosmeticOffset personalOffset;
};