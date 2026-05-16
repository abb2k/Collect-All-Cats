#include "BankruptPopup.hpp"

#include <layers/CatsLayer.hpp>
#include <nodes/SellCell.hpp>
#include <utils/Save.hpp>
#include <utils/CoinManager.hpp>
#include <hooks/DialogLayer.hpp>

BankruptPopup* BankruptPopup::create() {
    auto ret = new BankruptPopup();
    // @geode-ignore(unknown-resource)
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BankruptPopup::init() {
    // @geode-ignore(unknown-resource)
    if (!Popup::init(400, 250, "geode.loader/GE_square01.png")) return false;

    auto catsLayer = CatsLayer::activeCatLayer();
    if (!catsLayer) return false;

    auto scrollLayer = ScrollLayer::create(m_size / 1.2f - ccp(0, 20));
    scrollLayer->setPosition(m_size / 2);
    auto layout = RowLayout::create()
        ->setGrowCrossAxis(true)
        ->setCrossAxisAlignment(AxisAlignment::End)
        ->setGap(5);
    layout->ignoreInvisibleChildren(true);
    scrollLayer->m_contentLayer->setLayout(layout);
    scrollLayer->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChild(scrollLayer);

    auto scrollbar = Scrollbar::create(scrollLayer);
    scrollbar->setPosition(scrollLayer->getPosition() + ccp(scrollLayer->getContentWidth() / 2 + 5, 0));
    m_mainLayer->addChild(scrollbar);

    this->setTitle("Sell Items");

    this->m_closeBtn->setVisible(false);

    oldItemsBackup = Save::getUnlockedAccessories();

    for (const auto& entry : oldItemsBackup) {

        auto splitStr = utils::string::split(entry, "-");
        if (splitStr.size() != 2) continue;

        auto sellCell = SellCell::create(splitStr[0], splitStr[1]);
        if (sellCell == nullptr) continue;

        scrollLayer->m_contentLayer->addChild(sellCell);
    }

    scrollLayer->m_contentLayer->updateLayout();
    scrollLayer->moveToTop();

    auto scrollBG = CCScale9Sprite::create("square02_small.png");
    scrollBG->setContentSize(scrollLayer->getContentSize());
    scrollBG->setAnchorPoint({0, 0});
    scrollBG->setOpacity(80);
    scrollBG->setZOrder(-1);
    scrollLayer->addChild(scrollBG);

    auto bottoMenu = CCMenu::create();
    bottoMenu->setID("bottom-menu");
    bottoMenu->setContentSize({
        scrollLayer->getContentWidth(),
        20
    });
    bottoMenu->setPosition({
        m_size.width / 2,
        7.5f
    });
    bottoMenu->setAnchorPoint({0.5f, 0});
    bottoMenu->ignoreAnchorPointForPosition(false);
    bottoMenu->setLayout(SimpleAxisLayout::create(Axis::Row)
        ->setGap(80)
        ->setMainAxisScaling(AxisScaling::Scale)
        ->setMainAxisAlignment(MainAxisAlignment::Even)
    );
    m_mainLayer->addChild(bottoMenu);


    auto bailBtnSpr = ButtonSprite::create("Bail", "bigFont.fnt", "GJ_button_06.png");
    bailBtnSpr->setScale(.4f);
    auto bailBtn = CCMenuItemSpriteExtra::create(
        bailBtnSpr,
        this,
        menu_selector(BankruptPopup::onBail)
    );
    bottoMenu->addChild(bailBtn);

    auto confirmBtnSpr = ButtonSprite::create("Confirm", "bigFont.fnt", "GJ_button_01.png");
    confirmBtnSpr->setScale(.4f);
    auto confirmBtn = CCMenuItemSpriteExtra::create(
        confirmBtnSpr,
        this,
        menu_selector(BankruptPopup::onConfirm)
    );
    bottoMenu->addChild(confirmBtn);

    bottoMenu->updateLayout();

    return true;
}

void BankruptPopup::show(){
    Popup::show();
}

void BankruptPopup::onClose(CCObject*){
    Popup::onClose(nullptr);
}

void BankruptPopup::onBail(CCObject*){
    Save::saveUnlockedAccessories(oldItemsBackup);
    Popup::onClose(nullptr);
    CatsLayer::activeCatLayer()->keyBackClicked();
}

void BankruptPopup::onConfirm(CCObject*){
    auto array = CCArray::create();

    if (CoinManager::getCoinCount() < 0){
        array->addObject(DialogObject::create("The Cats", "You cannot enter until you get out of debt!", 999, 1, false, ccColor3B{ 255, 255, 255 }));
        array->addObject(DialogObject::create("The Cats", "Come on! Keep selling!", 999, 1, false, ccColor3B{ 255, 255, 255 }));

        auto dialogue = CACDialogLayer::createWithTaggedSprites(array, 1, {{"default_cat.png"_spr, 999, .65f}});
        this->addChild(dialogue, 100);
        return;
    }

    array->addObject(DialogObject::create("The Cats", "Thank you very much for your cooperation!", 999, 1, false, ccColor3B{ 255, 255, 255 }));
    array->addObject(DialogObject::create("The Cats", "You may now enter our home! As you no longer owe us anything!", 999, 1, false, ccColor3B{ 255, 255, 255 }));

    auto dialogue = CACDialogLayer::createWithTaggedSprites(array, 1, {{"default_cat.png"_spr, 999, .65f}});
    CatsLayer::activeCatLayer()->addChild(dialogue, 100);

    Popup::onClose(nullptr);
}