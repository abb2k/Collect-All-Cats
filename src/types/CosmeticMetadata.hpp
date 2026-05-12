#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CosmeticBoneOffset{
    std::string boneName;
    CCPoint offset;
};

struct CosmeticMetadata{
    std::string renderType; //Model/Texture
    std::map<std::string, CosmeticBoneOffset> boneOffsets; // category - {boneName, offset from bone}
    CCPoint personalOffset;
};