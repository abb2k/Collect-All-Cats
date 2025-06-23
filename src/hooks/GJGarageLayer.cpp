#include "GJGarageLayer.hpp"

#include "../layers/CatsLayer.hpp"

bool CACGarageLayer::init(){
    if (!GJGarageLayer::init()) return false;

    auto menu = CCMenu::create();
    this->addChild(menu);

    auto entryBtnSprite = CCSprite::createWithSpriteFrameName("abb2k_cac_closed.png"_spr);
    entryBtnSprite->setScale(.75f);
    auto entryBtn = CCMenuItemSpriteExtra::create(
        entryBtnSprite,
        nullptr,
        this,
        menu_selector(CACGarageLayer::OnMenuBtnClicked)
    );
    entryBtn->setPosition({254, -124});
    menu->addChild(entryBtn);
    
    return true;
}

void CACGarageLayer::OnMenuBtnClicked(CCObject*){
    auto scene = CCScene::create();

    auto catsLayer = CatsLayer::create();
    if (!catsLayer){
        auto dobj = DialogObject::create("The Cat Master", "You need to beat an <cr>extreme demon</c> to open this door...", 32, 1, true, ccColor3B{ 255, 255, 255 });
        auto dlayer = DialogLayer::createDialogLayer(dobj, nullptr, 6);
        dlayer->setZOrder(100);
        dlayer->m_mainLayer->setScale(0);
        dlayer->m_mainLayer->runAction(CCEaseBackOut::create(CCScaleTo::create(0.5f, 1)));
        this->addChild(dlayer);
        //
        return;
    }
    scene->addChild(catsLayer);

    CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
}