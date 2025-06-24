#include "CatSelectionCell.hpp"

#include "../utils/Save.hpp"
#include "LinkedCatDisplay.hpp"

CatSelectionCell* CatSelectionCell::create(GJGameLevel* level) {
    auto ret = new CatSelectionCell();
    if (ret->init(level)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CatSelectionCell::init(GJGameLevel* level) {
    if (!CCScale9Sprite::initWithFile("geode.loader/GE_square01.png")) return false;

    catsLayer = CatsLayer::activeCatLayer();
    if (!catsLayer) return false;

    this->setContentSize({122.5f, 122.5f});

    auto didLoadCat = Save::loadCat(level);
    if (didLoadCat.isErr()){
        myCatStats = CatStats::createDefault(level);
        auto didCatSave = Save::saveCat(myCatStats);
        if (didCatSave.isErr()){
            log::info("Failed to load cat!\n{}", didCatSave.unwrapErr());
            return false;
        }
    }
    else myCatStats = didLoadCat.unwrap();

    auto nameLabel = CCLabelBMFont::create(myCatStats.name.c_str(), "bigFont.fnt");
    nameLabel->setPosition({this->getContentWidth() / 2, 110});
    nameLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    nameLabel->setScale(std::min(0.75f, 100 / nameLabel->getContentWidth()));
    this->addChild(nameLabel);

    if (myCatStats.name != myCatStats.relatedLevel->m_levelName){
        auto levelNameLabel = CCLabelBMFont::create(myCatStats.relatedLevel->m_levelName.c_str(), "goldFont.fnt");
        levelNameLabel->setPosition({this->getContentWidth() / 2, 100});
        levelNameLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        levelNameLabel->setScale(std::min(0.5f, 75 / levelNameLabel->getContentWidth()));
        this->addChild(levelNameLabel);
    }

    auto catDisplay = LinkedCatDisplay::create();
    catDisplay->setScale(60.5f / catDisplay->getContentHeight());
    catDisplay->setPosition({this->getContentWidth() / 2, 55});
    this->addChild(catDisplay);

    auto buttonsMenu = CCMenu::create();
    buttonsMenu->setPosition({0, 0});
    this->addChild(buttonsMenu);

    auto catSettingsBtnSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    catSettingsBtnSprite->setScale(.5f);
    auto catSettingsBtn = CCMenuItemSpriteExtra::create(
        catSettingsBtnSprite,
        nullptr,
        this,
        menu_selector(CatSelectionCell::onCatSettingsClicked)
    );
    catSettingsBtn->setPosition(catSettingsBtn->getContentSize() / 2 + ccp(4, 6));
    buttonsMenu->addChild(catSettingsBtn);

    auto selectedToggleOnSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    selectedToggleOnSprite->setScale(.5f);
    auto selectedToggleOffSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    selectedToggleOffSprite->setScale(.5f);
    selectedToggle = CCMenuItemToggler::create(
        selectedToggleOffSprite,
        selectedToggleOnSprite,
        this,
        menu_selector(CatSelectionCell::onSelectedToggled)
    );
    selectedToggle->setPosition({108, 15});
    buttonsMenu->addChild(selectedToggle);

    auto placedCats = Save::getPlacedCats();

    for (const auto& catID : placedCats)
    {
        if (catID == myCatStats.relatedLevel->m_levelID.value()){
            selectedToggle->toggle(true);
            break;
        }
    }

    auto toggleLabel = CCLabelBMFont::create("Placed", "bigFont.fnt");
    toggleLabel->setScale(.2f);
    toggleLabel->setPosition({108, 27});
    this->addChild(toggleLabel);

    return true;
}

void CatSelectionCell::onCatSettingsClicked(CCObject*){
    CatsLayer::activeCatLayer()->catSettingsNode->setToCat(myCatStats);
    CatsLayer::activeCatLayer()->catSettingsNode->show();
}

void CatSelectionCell::onSelectedToggled(CCObject*){
    togglePlaced(!selectedToggle->isToggled(), false);
}

void CatSelectionCell::togglePlaced(bool placed, bool changeToggleSprite){
    auto placedCats = Save::getPlacedCats();
    std::set<int> placedCatsSet(placedCats.begin(), placedCats.end());
    bool containsMe = placedCatsSet.contains(myCatStats.relatedLevel->m_levelID.value());

    if (placed){
        if (!containsMe){
            Save::addPlacedCat(myCatStats.relatedLevel->m_levelID.value());
            catsLayer->addCat(myCatStats.relatedLevel);

            containsMe = true;
        }
    }
    else{
        Save::removePlacedCats(myCatStats.relatedLevel->m_levelID.value());
        catsLayer->removeCat(myCatStats.relatedLevel->m_levelID.value());

        containsMe = false;
    }

    if (changeToggleSprite)
        selectedToggle->toggle(containsMe);
}