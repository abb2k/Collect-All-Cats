#include "CatSelectionPopup.hpp"

#include <layers/CatsLayer.hpp>
#include <nodes/CatSelectionCell.hpp>

CatSelectionPopup* CatSelectionPopup::create() {
    auto ret = new CatSelectionPopup();
    // @geode-ignore(unknown-resource)
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
    auto layout = RowLayout::create()
        ->setGrowCrossAxis(true)
        ->setCrossAxisAlignment(AxisAlignment::End)
        ->setGap(5);
    layout->ignoreInvisibleChildren(true);
    catsScrollLayer->m_contentLayer->setLayout(layout);

    for (const auto& level : catsLayer->beatenExtremes)
    {
        auto cell = CatSelectionCell::create(level);
        allCells.push_back(cell);
        catsScrollLayer->m_contentLayer->addChild(cell);
    }

    std::sort(allCells.begin(), allCells.end(), [](CCMenu* aM, CCMenu* bM){ 
        auto a = typeinfo_cast<CatSelectionCell*>(aM);
        auto b = typeinfo_cast<CatSelectionCell*>(bM);
        return a->getStats().name < b->getStats().name;
    });

    auto scrollBar = Scrollbar::create(catsScrollLayer);
    scrollBar->setPosition(catsScrollLayer->getPosition() + ccp(catsScrollLayer->getContentWidth() + 5, catsScrollLayer->getContentHeight() / 2));
    m_mainLayer->addChild(scrollBar);

    selAllBtnSprite = ButtonSprite::create("Place All", "bigFont.fnt", "GJ_button_01.png");
    selAllBtnSprite->setScale(.4f);
    auto selAllBtn = CCMenuItemSpriteExtra::create(
        selAllBtnSprite,
        nullptr,
        this,
        menu_selector(CatSelectionPopup::onSelectAllClicked)
    );
    selAllBtn->setPosition({352, 278});
    m_buttonMenu->addChild(selAllBtn);

    deselAllBtnSprite = ButtonSprite::create("Remove All", "bigFont.fnt", "GJ_button_06.png");
    deselAllBtnSprite->setScale(.4f);
    auto deselAllBtn = CCMenuItemSpriteExtra::create(
        deselAllBtnSprite,
        nullptr,
        this,
        menu_selector(CatSelectionPopup::onDeselectAllClicked)
    );
    deselAllBtn->setPosition({267, 278});
    m_buttonMenu->addChild(deselAllBtn);

    auto searchBar = TextInput::create(m_size.width / 2.2f, "search", "bigFont.fnt");
    searchBar->setPosition({130, 277});
    searchBar->setCallback([&](const std::string& newStr){
        filter = newStr;
        CatSelectionPopup::updateCellVisibility();
    });
    m_mainLayer->addChild(searchBar);

    CatSelectionPopup::updateCellVisibility();

    CCTouchDispatcher::get()->addPrioTargetedDelegate(m_buttonMenu, -504, true);

    this->scheduleUpdate();

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

void CatSelectionPopup::fadeTo(GLubyte opacity, float time){
    for (const auto& cell : CCArrayExt<CatSelectionCell*>(catsScrollLayer->m_contentLayer->getChildren()))
        cell->runAction(CCFadeTo::create(time, opacity));
    
    m_buttonMenu->runAction(CCFadeTo::create(time, opacity));
    m_bgSprite->runAction(CCFadeTo::create(time, opacity));
    selAllBtnSprite->m_label->runAction(CCFadeTo::create(time, opacity));
    selAllBtnSprite->m_BGSprite->runAction(CCFadeTo::create(time, opacity));
    deselAllBtnSprite->m_label->runAction(CCFadeTo::create(time, opacity));
    deselAllBtnSprite->m_BGSprite->runAction(CCFadeTo::create(time, opacity));

    if (opacity != 255)
        this->runAction(CCFadeTo::create(time, 0));
    else
        this->runAction(CCFadeTo::create(time, 105));

}

std::vector<CCMenu*> CatSelectionPopup::getFilteredCells(){
    std::vector<CCMenu*> toReturn{};

    std::transform(filter.begin(), filter.end(), filter.begin(), ::tolower);
    if (filter == "") return allCells;

    for (const auto& cellMenu : allCells){
        auto cell = typeinfo_cast<CatSelectionCell*>(cellMenu);

        auto nickname = cell->getStats().name;
        std::transform(nickname.begin(), nickname.end(), nickname.begin(), ::tolower);
        auto lvlname = cell->getStats().getLevel()->m_levelName;
        std::transform(lvlname.begin(), lvlname.end(), lvlname.begin(), ::tolower);
 
        if (!CatSelectionPopup::containsWord(nickname, filter) && !CatSelectionPopup::containsWord(lvlname, filter)) continue;

        toReturn.push_back(cell);
    }

    return toReturn;
}

void CatSelectionPopup::updateCellVisibility(){
    auto filteredList = CatSelectionPopup::getFilteredCells();

    std::vector<CCMenu*> allCellsUnvisited = allCells;

    isVisible.clear();

    for (const auto& cellMenu : filteredList){
        allCellsUnvisited.erase(find(allCellsUnvisited.begin(), allCellsUnvisited.end(), cellMenu));
        
        isVisible.insert({cellMenu, true});
    }

    for (const auto& cellMenu : allCellsUnvisited){
        isVisible.insert({cellMenu, false});
    }
}

void CatSelectionPopup::update(float dt){
    for (const auto& cellMenu : allCells){
        if (!isVisible.contains(cellMenu)) continue;

        cellMenu->setVisible(isVisible[cellMenu]);
    }

    catsScrollLayer->m_contentLayer->updateLayout();

    if (catsScrollLayer->m_contentLayer->getContentHeight() < catsScrollLayer->getContentHeight()){
        catsScrollLayer->setTouchEnabled(false);
        catsScrollLayer->enableScrollWheel(false);
    }
    else{
        catsScrollLayer->setTouchEnabled(true);
        catsScrollLayer->enableScrollWheel(true);
    }

    if (prevHeight != catsScrollLayer->m_contentLayer->getContentHeight()){
        prevHeight = catsScrollLayer->m_contentLayer->getContentHeight();
        catsScrollLayer->moveToTop();
    }
}

bool CatSelectionPopup::containsWord(const std::string& str, const std::string& word){
    auto pos = str.find(word);
    if (pos == std::string::npos) return false;

    return pos == 0 || str[pos - 1] == ' ';
}