#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RoomSettingsPopup : public Popup<>, public SelectArtDelegate, public ColorPickPopupDelegate {
    public:
        static RoomSettingsPopup* create();

    private:
        virtual bool setup() override;

        virtual void selectArtClosed(SelectArtLayer* selectLater) override;
        virtual void updateColor(cocos2d::ccColor4B const& color) override;

        void openBGSelect(CCObject*);
        void openGroundSelect(CCObject*);

        void updateBGButton(int BGID);
        void updateGroundButton(int GroundID);

        SelectArtLayer* BGSelectLayer = nullptr;
        SelectArtLayer* groundSelectLayer = nullptr;

        CCMenuItemSpriteExtra* BGSelectBtn;
        CCMenuItemSpriteExtra* GroundSelectBtn;

        CCSprite* BGColorSelectBtnSprite;
        CCSprite* GroundColorSelectBtnSprite;

        void openBGColorSelect(CCObject*);
        void openGroundColorSelect(CCObject*);
        int lastClickedColorBtn = 0;
};
