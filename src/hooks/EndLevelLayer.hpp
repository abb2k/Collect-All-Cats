#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/EndLevelLayer.hpp>

class $modify(CACEndLevelLayer, EndLevelLayer) {
    void customSetup();
    void playEndEffect();
    void catEnter();
    void spawnCircleWave();

    struct Fields{
        CCNode* catContainer;
    };
};