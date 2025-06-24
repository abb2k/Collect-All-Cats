#include "CatSettingsNode.hpp"
#include "../layers/CatsLayer.hpp"

CatSettingsNode* CatSettingsNode::create() {
    auto ret = new CatSettingsNode();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CatSettingsNode::init() {
    CCTouchDispatcher::get()->registerForcePrio(this, 3);
    if (!CCLayer::init()) return false;

    auto winSize = CCDirector::get()->getWinSize();

    this->setContentSize({250, winSize.height});
    this->setPositionX(winSize.width);
    this->setAnchorPoint({0, 0});

    auto BG = CCScale9Sprite::create("geode.loader/GE_square03.png");
    BG->setAnchorPoint({0, 0});
    BG->setContentSize(this->getContentSize());
    this->addChild(BG);

    buttonsMenu = CCMenu::create();
    buttonsMenu->setContentSize(this->getContentSize() - ccp(30, 0));
    buttonsMenu->setPosition({0, 0});
    buttonsMenu->setZOrder(100);
    buttonsMenu->setEnabled(false);
    buttonsMenu->setTouchPriority(-1);
    this->addChild(buttonsMenu);

    auto backBtnSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    backBtnSprite->setScale(0.75f);
    backBtnSprite->setFlipX(true);
    auto backBtn = CCMenuItemSpriteExtra::create(
        backBtnSprite,
        nullptr,
        this,
        menu_selector(CatSettingsNode::onBackClicked)
    );
    backBtn->setPosition({backBtn->getContentWidth() / 2 + 10, buttonsMenu->getContentHeight() - backBtn->getContentHeight() / 2 - 5});
    buttonsMenu->addChild(backBtn);

    catDisplay = LinkedCatDisplay::create();
    catDisplay->setPosition({buttonsMenu->getContentWidth() / 2, buttonsMenu->getContentHeight() - catDisplay->getContentHeight() / 2 - 10});
    catDisplay->setScale(110.0f / catDisplay->getContentHeight());
    this->addChild(catDisplay);

    auto blockerMenu = CCMenu::create();
    blockerMenu->setContentSize(this->getContentSize() - ccp(30, 0));
    blockerMenu->setPosition({0, 0});
    blockerMenu->setZOrder(100);
    blockerMenu->setEnabled(true);
    this->addChild(blockerMenu);

    auto blocker = CCMenuItem::create();
    blocker->setContentSize(blockerMenu->getContentSize());
    blocker->setAnchorPoint({0, 0});
    blockerMenu->addChild(blocker);

    this->setTouchEnabled(false);
    this->setKeyboardEnabled(false);
    this->setKeypadEnabled(false);
    
    return true;
}

void CatSettingsNode::show(){
    if (isOpen) return;
    isOpen = true;
    this->runAction(CCEaseBackOut::create(CCMoveBy::create(0.4f, ccp(-this->getContentWidth() + 30, 0))));

    this->setTouchEnabled(true);
    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);
    buttonsMenu->setEnabled(true);
    handleTouchPriorityWith(this, -600, true);

    auto selPopup = CatsLayer::activeCatLayer()->currentSelectionPopup;
    if (!selPopup) return;

    selPopup->easeHorizontal(CCMoveBy::create(0.3f, ccp(-60, 0)));
}

void CatSettingsNode::hide(){
    if (!isOpen) return;
    isOpen = false;
    this->runAction(CCEaseBackIn::create(CCMoveBy::create(0.4f, ccp(this->getContentWidth() - 30, 0))));

    this->setTouchEnabled(false);
    this->setKeyboardEnabled(false);
    this->setKeypadEnabled(false);
    buttonsMenu->setEnabled(false);

    auto selPopup = CatsLayer::activeCatLayer()->currentSelectionPopup;
    if (!selPopup) return;

    selPopup->easeHorizontal(CCMoveBy::create(0.3f, ccp(60, 0)));
}

void CatSettingsNode::keyBackClicked(){
    hide();
}

void CatSettingsNode::setToCat(const CatStats& stats){
    catToModify = stats;
}

void CatSettingsNode::onCatApplyCallback(const std::function<void(const CatStats&)>& callback){
    this->callback = callback;
}

void CatSettingsNode::onBackClicked(CCObject*){
    keyBackClicked();
}