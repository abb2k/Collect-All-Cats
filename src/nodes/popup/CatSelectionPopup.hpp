#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <types/CatStats.hpp>

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

        std::vector<CatStats> allLevels{};
        std::string filter = "";

        bool containsWord(const std::string& str, const std::string& word);

        int currentPage = 0;
        const int CATS_PER_PAGE = 21;
        std::vector<CatStats*> getFilteredCats();
        std::vector<CatStats*> getPageCats();

        void updatePageContent();
        float prevHeight;
};
