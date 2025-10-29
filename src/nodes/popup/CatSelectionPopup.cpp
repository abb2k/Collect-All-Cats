#include "CatSelectionPopup.hpp"

#include <layers/CatsLayer.hpp>
#include <nodes/CatSelectionCell.hpp>

#include <utils/Save.hpp>

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

    catsScrollLayer = ScrollLayer::create({375, 250 * 1.1f});
    catsScrollLayer->setScale(.9f);
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
        auto didGetCat = Save::loadCatOrDefault(level);

        if (didGetCat.isOk())
            allLevels.push_back(didGetCat.unwrap());
        else
            log::error("{}", didGetCat.unwrapErr());
    }

    std::sort(allLevels.begin(), allLevels.end(), [](const CatStats& a, const CatStats& b){ 
        return a.name < b.name;
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
        currentPage = 0;
        CatSelectionPopup::updatePageContent();
    });
    m_mainLayer->addChild(searchBar);

    CatSelectionPopup::updatePageContent();

    auto pageLeftBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    pageLeftBtnSpr->setScale(.5f);
    pageLeftBtn = CCMenuItemSpriteExtra::create(
        pageLeftBtnSpr,
        this,
        menu_selector(CatSelectionPopup::movePage)
    );
    pageLeftBtn->setPosition({17.5f, catsScrollLayer->getContentHeight() / 2});
    m_buttonMenu->addChild(pageLeftBtn);

    auto pageRightBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    pageRightBtnSpr->setFlipX(true);
    pageRightBtnSpr->setScale(.5f);
    pageRightBtn = CCMenuItemSpriteExtra::create(
        pageRightBtnSpr,
        this,
        menu_selector(CatSelectionPopup::movePage)
    );
    pageRightBtn->setPosition({375, catsScrollLayer->getContentHeight() / 2});
    m_buttonMenu->addChild(pageRightBtn);

    CCTouchDispatcher::get()->addPrioTargetedDelegate(m_buttonMenu, -504, true);

    this->scheduleUpdate();

    CatSelectionPopup::movePage(nullptr);

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

bool CatSelectionPopup::containsWord(const std::string& str, const std::string& word){
    auto pos = str.find(word);
    if (pos == std::string::npos) return false;

    return pos == 0 || str[pos - 1] == ' ';
}

std::vector<CatStats*> CatSelectionPopup::getFilteredCats(){
    std::vector<CatStats*> toReturn{};

    auto makeLower = [](std::string str) -> std::string {
        for (size_t i = 0; i < str.length(); ++i) {
            str[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(str[i])));
        }
        return str;
    };

    auto lowerFilter = makeLower(filter);
    if (lowerFilter == ""){
        for (auto& catStat : allLevels)
            toReturn.push_back(&catStat);
        return toReturn;
    }

    for (auto& stats : allLevels){

        auto nickname = makeLower(stats.name);
        auto lvlname = makeLower(stats.getLevel()->m_levelName);
 
        if (!CatSelectionPopup::containsWord(nickname, lowerFilter) && !CatSelectionPopup::containsWord(lvlname, lowerFilter)) continue;

        toReturn.push_back(&stats);
    }

    return toReturn;
}

std::vector<CatStats*> CatSelectionPopup::getPageCats(){
    std::vector<CatStats*> toReturn{};

    auto list = CatSelectionPopup::getFilteredCats();

    auto startIdx = currentPage * CATS_PER_PAGE;
    auto endIdx = std::min(startIdx + CATS_PER_PAGE, static_cast<int>(list.size()));

    for (int i = startIdx; i < endIdx; i++)
        toReturn.push_back(list[i]);

    return toReturn;
}

void CatSelectionPopup::updatePageContent(){
    catsScrollLayer->m_contentLayer->removeAllChildren();

    auto pageCats = CatSelectionPopup::getPageCats();

    for (const auto& catStatsPtr : pageCats)
    {
        auto cell = CatSelectionCell::create(*catStatsPtr);
        catsScrollLayer->m_contentLayer->addChild(cell);
    }

    catsScrollLayer->m_contentLayer->updateLayout();
    catsScrollLayer->moveToTop();

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

int CatSelectionPopup::getPageCount(){
    auto filteredCats = CatSelectionPopup::getFilteredCats();
    return (filteredCats.size() + CATS_PER_PAGE - 1) / CATS_PER_PAGE;
}

void CatSelectionPopup::movePage(CCObject* sender){
    if (sender == pageLeftBtn){
        if (currentPage > 0){
            currentPage--;
            CatSelectionPopup::updatePageContent();
        }
    }
    else if (sender == pageRightBtn){
        if (currentPage + 1 < CatSelectionPopup::getPageCount()){
            currentPage++;
            CatSelectionPopup::updatePageContent();
        }
    }

    if (currentPage == 0){
        pageLeftBtn->setOpacity(PAGE_ARROW_DISABLED_OPACITY);
        pageLeftBtn->setEnabled(false);
    }
    else{
        pageLeftBtn->setOpacity(255);
        pageLeftBtn->setEnabled(true);
    }

    if (currentPage == CatSelectionPopup::getPageCount() - 1){
        pageRightBtn->setOpacity(PAGE_ARROW_DISABLED_OPACITY);
        pageRightBtn->setEnabled(false);
    }
    else{
        pageRightBtn->setOpacity(255);
        pageRightBtn->setEnabled(true);
    }
}