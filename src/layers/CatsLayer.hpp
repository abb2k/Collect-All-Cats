#pragma once

#include <Geode/Geode.hpp>
#include "../nodes/GroundLoader.hpp"
#include "../nodes/BGLoader.hpp"
#include "../nodes/Cat.hpp"
#include "../nodes/CatSettingsNode.hpp"
#include "../nodes/popup/CatSelectionPopup.hpp"

using namespace geode::prelude;

class CatsLayer : public CCLayer {
    public:
        static CatsLayer* create();

        static CatsLayer* activeCatLayer();

        void onBackClicked(CCObject*);
        void onCatsMenuClicked(CCObject*);
        void onSettingsClicked(CCObject*);

        ScrollLayer* ScrollNode;

        GroundLoader* groundLoader;
        BGLoader* bgLoader;

        std::vector<GJGameLevel*> beatenExtremes{};

        CatSettingsNode* catSettingsNode = nullptr;
        CatSelectionPopup* currentSelectionPopup = nullptr;

        std::map<int, Cat*> spawnedCats{};
        void addCat(GJGameLevel* catLevel);
        void removeCat(int catID);

        void createCatSettingsNode(CCScene* scene);
    private:
        virtual bool init();

        virtual void keyBackClicked();

        static CatsLayer* sharedInstance;

        void update(float dt);
};