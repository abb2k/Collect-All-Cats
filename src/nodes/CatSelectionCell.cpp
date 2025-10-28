#include "CatSelectionCell.hpp"

#include <utils/Save.hpp>
#include <nodes/LinkedCatDisplay.hpp>

CatSelectionCell* CatSelectionCell::create(const CatStats& stats) {
    auto ret = new CatSelectionCell();
    if (ret->init(stats)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CatSelectionCell::init(const CatStats& stats) {
    if (!CCMenu::init()) return false;

    catsLayer = CatsLayer::activeCatLayer();
    if (!catsLayer) return false;

    this->setContentSize({122.5f, 122.5f});

    // @geode-ignore(unknown-resource)
    auto BG = CCScale9Sprite::create("geode.loader/GE_square01.png");
    BG->setContentSize(this->getContentSize());
    BG->setZOrder(-1);
    BG->setAnchorPoint({0, 0});
    this->addChild(BG);

    myCatStats = stats;

    nameLabel = CCLabelBMFont::create(myCatStats.name.c_str(), "bigFont.fnt");
    nameLabel->setID("name-label");
    nameLabel->setPosition({this->getContentWidth() / 2, 110});
    nameLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    nameLabel->setScale(std::min(0.75f, 100 / nameLabel->getContentWidth()));
    this->addChild(nameLabel);

    levelNameLabel = CCLabelBMFont::create(myCatStats.getLevel()->m_levelName.c_str(), "goldFont.fnt");
    levelNameLabel->setID("level-name-label");
    levelNameLabel->setPosition({this->getContentWidth() / 2, 95});
    levelNameLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    levelNameLabel->setScale(std::min(0.5f, 75 / levelNameLabel->getContentWidth()));
    this->addChild(levelNameLabel);

    if (myCatStats.name == myCatStats.getLevel()->m_levelName){
        levelNameLabel->setString("");
    }

    auto catDisplay = LinkedCatDisplay::create(&myCatStats);
    catDisplay->setID("cat-display");
    catDisplay->setScale(60.5f / catDisplay->getContentHeight());
    catDisplay->setPosition({this->getContentWidth() / 2, 55});
    this->addChild(catDisplay);

    auto catSettingsBtnSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    catSettingsBtnSprite->setScale(.5f);
    auto catSettingsBtn = CCMenuItemSpriteExtra::create(
        catSettingsBtnSprite,
        nullptr,
        this,
        menu_selector(CatSelectionCell::onCatSettingsClicked)
    );
    catSettingsBtn->setID("cat-settings-btn");
    catSettingsBtn->setPosition(catSettingsBtn->getContentSize() / 2 + ccp(4, 6));
    this->addChild(catSettingsBtn);

    auto placedCats = Save::getPlacedCats();
    bool catPlaced = false;

    for (const auto& catID : placedCats)
    {
        if (catID == myCatStats.getLevel()->m_levelID.value()){
            catPlaced = true;
            break;
        }
    }

    selectedToggle = SimpleToggler::createWithDefaults(
        .5f,
        catPlaced
    );
    selectedToggle->setID("cat-visible-toggle");
    selectedToggle->setCallback([&](bool state){CatSelectionCell::togglePlaced(state, false);});
    selectedToggle->setPosition({108, 15});
    this->addChild(selectedToggle);

    auto toggleLabel = CCLabelBMFont::create("Placed", "bigFont.fnt");
    toggleLabel->setID("cat-visible-toggle-label");
    toggleLabel->setScale(.2f);
    toggleLabel->setPosition({108, 27});
    this->addChild(toggleLabel);

    return true;
}

void CatSelectionCell::onCatSettingsClicked(CCObject*){
    CatSelectionCell::getLatestStats();
    
    CatsLayer::activeCatLayer()->catSettingsNode->showWithCat(myCatStats);
}

void CatSelectionCell::togglePlaced(bool placed, bool changeToggleSprite){
    CatSelectionCell::getLatestStats();

    auto placedCats = Save::getPlacedCats();
    std::set<int> placedCatsSet(placedCats.begin(), placedCats.end());
    bool containsMe = placedCatsSet.contains(myCatStats.getLevel()->m_levelID.value());

    if (placed){
        if (!containsMe){
            Save::addPlacedCat(myCatStats.getLevel()->m_levelID.value());
            catsLayer->addCat(myCatStats.getLevel());

            containsMe = true;
        }
    }
    else{
        Save::removePlacedCats(myCatStats.getLevel()->m_levelID.value());
        catsLayer->removeCat(myCatStats.getLevel()->m_levelID.value());

        containsMe = false;
    }

    if (changeToggleSprite)
        selectedToggle->toggle(containsMe);
}

void CatSelectionCell::onStatsChanged(const CatStats& newStats){
    myCatStats = newStats;
    nameLabel->setString(newStats.name.c_str());
    nameLabel->setScale(std::min(0.75f, 100 / nameLabel->getContentWidth()));
    
    if (myCatStats.name != myCatStats.getLevel()->m_levelName){
        levelNameLabel->setString(myCatStats.getLevel()->m_levelName.c_str());
        levelNameLabel->setScale(std::min(0.5f, 75 / levelNameLabel->getContentWidth()));
    }
    else levelNameLabel->setString("");

    if (catsLayer->spawnedCats.contains(myCatStats.getLevel()->m_levelID.value())){
        catsLayer->spawnedCats[myCatStats.getLevel()->m_levelID.value()]->setCatStats(myCatStats);
    }
    else auto _ = Save::saveCat(&myCatStats);
}

void CatSelectionCell::getLatestStats(){
    if (myCatStats.isEmpty()) return;

    auto loadRes = Save::loadCat(myCatStats.getLevel());
    if (loadRes.isErr()) return;

    myCatStats = loadRes.unwrap();
}