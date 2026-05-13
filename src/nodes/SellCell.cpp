#include "SellCell.hpp"

#include <nodes/CatagoryAssetDisplay.hpp>
#include <nodes/OptionSwitcher.hpp>

SellCell* SellCell::create(const std::string& itemCategory, const std::string& itemID){
    auto ret = new SellCell();
    if (ret && ret->init(itemCategory, itemID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SellCell::init(const std::string& itemCategory, const std::string& itemID){
    if (!CCMenu::init()) return false;

    this->setContentSize({70,100});

    auto bg = CCScale9Sprite::create("GJ_square04.png");
    bg->setOpacity(100);
    bg->setScale(.5f);
    bg->setContentSize(this->getContentSize() / bg->getScale());
    bg->setAnchorPoint({0, 0});
    this->addChild(bg);

    auto display = CatagoryAssetDisplay::create();
    display->setAsset(itemCategory, geode::utils::numFromString<int>(itemID).unwrapOr(0));
    display->setAnchorPoint({.5f, 1});
    this->addChild(display);

    display->setAssetUpdatedCallback([&](CatagoryAssetDisplay* display){
        auto meta = display->getMetadata();

        auto coinSpr = CCSprite::createWithSpriteFrameName("secretCoinUI2_001.png");
        coinSpr->setAnchorPoint({1, .5f});
        coinSpr->setScale(.3f);
        coinSpr->setCascadeColorEnabled(false);
        coinSpr->setID("coin-spr");
        this->addChild(coinSpr);

        auto coinCountLabel = CCLabelBMFont::create(fmt::format("{}", meta.cost).c_str(), "bigFont.fnt");
        coinCountLabel->setAnchorPoint({0, .5f});
        coinCountLabel->setScale(coinSpr->getScale());
        coinCountLabel->setID("coin-label");
        this->addChild(coinCountLabel);

        auto switcher = OptionSwitcher<bool>::create(this->getContentWidth() / 1.15f * 2, {
            NamedEnumValue<bool>(false, "Sell"),
            NamedEnumValue<bool>(true, "Retrieve")
        });
        switcher->setScale(.5f);
        switcher->setCallback([&](bool isRetrieve){

        });
        switcher->setAnchorPoint({.5f, 0});
        switcher->setPosition({this->getContentWidth() / 2, 4});
        this->addChild(switcher);

        coinSpr->setPosition({this->getContentWidth() / 2 - coinCountLabel->getScaledContentWidth() / 2, 4 + 3 + switcher->getScaledContentHeight() + coinSpr->getScaledContentHeight() / 2});
        coinCountLabel->setPosition({this->getContentWidth() / 2 - coinCountLabel->getScaledContentWidth() / 2, 4 + 3 + switcher->getScaledContentHeight() + coinSpr->getScaledContentHeight() / 2});

        display->setPosition({this->getContentWidth() / 2, this->getContentHeight() - 5});
        display->setScale((this->getContentWidth() - 5) / display->getContentWidth());
    });

    return true;
}
