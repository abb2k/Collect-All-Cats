#pragma once

#include "CatSettingsLayer.hpp"

#include <Geode/Geode.hpp>
#include <nodes/GroundLoader.hpp>
#include <nodes/BGLoader.hpp>
#include <nodes/Cat.hpp>
#include <nodes/popup/CatSelectionPopup.hpp>
#include <nodes/AdvancedScrollLayer.hpp>

using namespace geode::prelude;

class CatsLayer : public CCLayer {
    public:
        static CatsLayer* create();

        static CatsLayer* activeCatLayer();

        void onBackClicked(CCObject*);
        void onCatsMenuClicked(CCObject*);
        void onSettingsClicked(CCObject*);
        void onEditorClicked(CCObject*);

        AdvancedScrollLayer* ScrollNode;

        GroundLoader* groundLoader;
        BGLoader* bgLoader;

        std::vector<GJGameLevel*> beatenExtremes{};

        CatSettingsLayer* catSettingsNode = nullptr;
        CatSelectionPopup* currentSelectionPopup = nullptr;

        std::map<int, Cat*> spawnedCats{};
        void addCat(GJGameLevel* catLevel);
        void removeCat(int catID);

        void createCatSettingsNode(CCScene* scene);

        void setFollowTarget(Cat* cat);
        Cat* getCatFromStats(const CatStats& stats);

        const bool getIsInEditor(){return isInEditor;}
    private:
        virtual bool init();

        virtual void keyBackClicked();

        static CatsLayer* sharedInstance;

        void update(float dt);

        Cat* followTarget = nullptr;

        CCNode* catContainer;

        void followUpdate(float dt);

        CCMenuItemSpriteExtra* editorModeBtn;
        CCMenu* topRightMenu;
        CCSprite* editorModeBtnSpr;
        CCSprite* newEditorSpr;

        bool isInEditor;
};