#include "CatSelectionPopup.hpp"
#include "../../layers/CatsLayer.hpp"
#include"../CatSelectionCell.hpp"

CatSelectionPopup* CatSelectionPopup::create() {
    auto ret = new CatSelectionPopup();
    if (ret->initAnchored(400, 300, "geode.loader/GE_square03.png")) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CatSelectionPopup::setup() {

    auto catsLayer = CatsLayer::activeCatLayer();
    if (!catsLayer) return false;
    
    m_bgSprite->setOpacity(220);
    m_closeBtn->setPosition({m_closeBtn->getPositionX() + m_closeBtn->getContentWidth() / 2, m_closeBtn->getPositionY() - m_closeBtn->getContentHeight() / 2});
    setCloseButtonSpr(CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"), .75f);

    catsScrollLayer = ScrollLayer::create({375, 250});
    catsScrollLayer->setPosition((m_size - catsScrollLayer->getContentSize()) / 2 - ccp(3.5f, 15));
    m_mainLayer->addChild(catsScrollLayer);
    catsScrollLayer->m_contentLayer->setLayout(RowLayout::create()
        ->setGrowCrossAxis(true)
        ->setCrossAxisAlignment(AxisAlignment::End)
        ->setGap(5)
    );

    for (const auto& level : catsLayer->beatenExtremes)
    {
        auto cell = CatSelectionCell::create(level);
        catsScrollLayer->m_contentLayer->addChild(cell);
    }

    catsScrollLayer->m_contentLayer->updateLayout();
    catsScrollLayer->moveToTop();

    auto scrollBar = Scrollbar::create(catsScrollLayer);
    scrollBar->setPosition(catsScrollLayer->getPosition() + ccp(catsScrollLayer->getContentWidth() + 5, catsScrollLayer->getContentHeight() / 2));
    m_mainLayer->addChild(scrollBar);

    auto selAllBtnSprite = ButtonSprite::create("Place All", "bigFont.fnt", "GJ_button_01.png");
    selAllBtnSprite->setScale(.65f);
    auto selAllBtn = CCMenuItemSpriteExtra::create(
        selAllBtnSprite,
        nullptr,
        this,
        menu_selector(CatSelectionPopup::onSelectAllClicked)
    );
    selAllBtn->setPosition({327, 278});
    m_buttonMenu->addChild(selAllBtn);

    auto deselAllBtnSprite = ButtonSprite::create("Remove All", "bigFont.fnt", "GJ_button_06.png");
    deselAllBtnSprite->setScale(.65f);
    auto deselAllBtn = CCMenuItemSpriteExtra::create(
        deselAllBtnSprite,
        nullptr,
        this,
        menu_selector(CatSelectionPopup::onDeselectAllClicked)
    );
    deselAllBtn->setPosition({190, 278});
    m_buttonMenu->addChild(deselAllBtn);

    return true;
}

void CatSelectionPopup::show(){
    Popup<>::show();
    this->setZOrder(this->getZOrder() - 10);
}

void CatSelectionPopup::onSelectAllClicked(CCObject*){
    for (const auto& catSelCell : CCArrayExt<CatSelectionCell*>(catsScrollLayer->m_contentLayer->getChildren()))
        catSelCell->togglePlaced(true, true);
}

void CatSelectionPopup::onDeselectAllClicked(CCObject*){
    for (const auto& catSelCell : CCArrayExt<CatSelectionCell*>(catsScrollLayer->m_contentLayer->getChildren()))
        catSelCell->togglePlaced(false, true);
}

void CatSelectionPopup::onClose(CCObject*){
    CatsLayer::activeCatLayer()->catSettingsNode->hide();
    CatsLayer::activeCatLayer()->currentSelectionPopup = nullptr;
    Popup<>::onClose(nullptr);
}

void CatSelectionPopup::easeHorizontal(CCMoveBy* move){
    m_mainLayer->runAction(CCEaseInOut::create(move, 2));
}
