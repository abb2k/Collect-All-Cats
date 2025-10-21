#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/GJGarageLayer.hpp>

class $modify(CACGarageLayer, GJGarageLayer) {
    bool init();

    void OnMenuBtnClicked(CCObject*);

    void startDialogue();

    void dialogClosed(DialogLayer* dialogueLayer);

    void openDoor();
    void postDoorOpenDialogue();

    struct Fields{
        CCLayer* evilPrioStealer;
        DialogLayer* dialogue = nullptr;
        DialogLayer* catsTestDialogue = nullptr;
        CCMenuItemSpriteExtra* entryBtn;
        DialogLayer* catsPostDoorDialogue = nullptr;
    };
};