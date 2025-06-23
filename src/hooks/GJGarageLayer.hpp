#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/GJGarageLayer.hpp>

class $modify(CACGarageLayer, GJGarageLayer) {
    bool init();

    void OnMenuBtnClicked(CCObject*);
};