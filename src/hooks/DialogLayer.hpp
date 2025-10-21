#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/DialogLayer.hpp>

class $modify(CACDialogLayer, DialogLayer) {
    void useTaggedSprites(const std::vector<std::tuple<std::string, int, float>>& icons);

    void displayDialogObject(DialogObject* dialogue);

    static DialogLayer* createWithTaggedSprites(CCArray* objects, int bg, const std::vector<std::tuple<std::string, int, float>>& icons);

    struct Fields{
        std::vector<std::tuple<std::string, int, float>> icons;
    };
};