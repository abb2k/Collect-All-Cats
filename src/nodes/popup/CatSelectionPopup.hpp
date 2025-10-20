#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;


class CatSelectionPopup : public Popup<> {
    public:
        static CatSelectionPopup* create();

        virtual void show() override;

        void fadeTo(GLubyte opacity, float time);
    private:
        virtual bool setup() override;

        void onSelectAllClicked(CCObject*);
        void onDeselectAllClicked(CCObject*);

        virtual void onClose(CCObject*) override;

        ScrollLayer* catsScrollLayer;

        ButtonSprite* selAllBtnSprite;
        ButtonSprite* deselAllBtnSprite;

        std::vector<CCMenu*> allCells{};
        std::string filter = "";
        float prevHeight;
        std::vector<CCMenu*> getFilteredCells();
        std::map<CCMenu*, bool> isVisible{};

        void updateCellVisibility();

        void update(float dt) override;

        bool containsWord(const std::string& str, const std::string& word);
};
