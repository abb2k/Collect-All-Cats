#include "NewCatsLayer.hpp"

#include <nodes/LinkedCatDisplay.hpp>

NewCatsLayer* NewCatsLayer::create(const std::vector<CatStats>& newCats) {
    auto ret = new NewCatsLayer();
    if (ret->init(newCats)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool NewCatsLayer::init(const std::vector<CatStats>& newCats) {
    if (!CCLayer::init()) return false;

    auto lightningFrames = CCArray::create();
    auto frameCache = CCSpriteFrameCache::get();
    CCSize frameSize;
    for (int i = 1; i < 11; i++) {
        int tens = i / 10;
        int nums = i - 10 * tens;

        auto frame = frameCache->spriteFrameByName(fmt::format("gj_lightning02_0{}{}.png", tens, nums).c_str());

        lightningFrames->addObject(frame);

        frameSize = frame->getOriginalSize();
    }
    lightningFrames->addObject(frameCache->spriteFrameByName("blank.png"_spr));

    auto winSize = CCDirector::get()->getWinSize();
    
    auto lightning = CCSprite::create();
    lightning->setID("effect-lightning");
    lightning->setPosition(winSize / 2 + ccp(winSize.width / frameSize.width / .25f, 0));
    lightning->setScaleY(3.5f);
    lightning->setScaleX(winSize.width / frameSize.width * 1.5f);
    this->addChild(lightning, 20);

    lighningAnim = CCAnimation::createWithSpriteFrames(lightningFrames, 0.03f);

    auto shadow = CCSprite::create("square.png");
    shadow->setID("layer-shadow");
    shadow->setScaleX(winSize.width / shadow->getContentWidth());
    shadow->setScaleY(winSize.height / shadow->getContentHeight());
    shadow->setColor({0,0,0});
    shadow->setOpacity(0);
    shadow->setPosition(winSize / 2);
    this->addChild(shadow);

    shadow->runAction(CCFadeTo::create(1, 150));

    lightning->runAction(CCSequence::create(CCDelayTime::create(1), CCAnimate::create(lighningAnim), nullptr));

    auto kittyAlignmentMenu = CCNode::create();
    kittyAlignmentMenu->setID("new-cats-container");
    kittyAlignmentMenu->setPosition(winSize / 2);
    kittyAlignmentMenu->setContentSize({winSize.width, 100});
    kittyAlignmentMenu->setAnchorPoint({.5f, .5f});
    kittyAlignmentMenu->setLayout(SimpleAxisLayout::create(Axis::Row)
        ->setCrossAxisScaling(AxisScaling::ScaleDownGaps)
        ->setGap(15)
        ->setMinRelativeScale(false)
    );
    this->addChild(kittyAlignmentMenu);

    auto kittyMenuShadow = CCSprite::create("square.png");
    kittyMenuShadow->setID("new-cats-bg-shadow");
    kittyMenuShadow->setPosition(kittyAlignmentMenu->getPosition());
    kittyMenuShadow->setScaleX(kittyAlignmentMenu->getContentWidth() / kittyMenuShadow->getContentWidth());
    kittyMenuShadow->setAnchorPoint(kittyAlignmentMenu->getAnchorPoint());
    kittyMenuShadow->setColor({0,0,0});
    kittyMenuShadow->setOpacity(150);
    kittyMenuShadow->setZOrder(-1);
    kittyMenuShadow->setScaleY(0);
    this->addChild(kittyMenuShadow);

    for (const auto& newCat : newCats)
    {
        auto tempCat = newCat;
        auto display = LinkedCatDisplay::create(&tempCat);
        kittyAlignmentMenu->addChild(display);
    }

    kittyAlignmentMenu->updateLayout();

    kittyAlignmentMenu->setScaleY(0);
    kittyAlignmentMenu->runAction(CCSequence::create(CCDelayTime::create(1.1f), CCEaseExponentialOut::create(CCScaleTo::create(.3f, 1)), nullptr));
    kittyMenuShadow->runAction(CCSequence::create(CCDelayTime::create(1.05f), CCEaseExponentialOut::create(CCScaleTo::create(.3f, kittyMenuShadow->getScaleX(), kittyAlignmentMenu->getContentHeight() / kittyMenuShadow->getContentHeight())), nullptr));

    kittyAlignmentMenu->setSkewY(20);
    kittyAlignmentMenu->runAction(CCSequence::create(CCDelayTime::create(1.1f), CCEaseBackOut::create(CCSkewTo::create(.5f, 0, 0)), nullptr));

    auto title = CCLabelBMFont::create("New Cats!", "bigFont.fnt");
    title->setID("new-cats-title");
    title->setScaleX(1.5f);
    title->setScaleY(0);
    title->setPosition({winSize.width / 2, winSize.height / 1.2f});
    this->addChild(title);

    title->runAction(CCSequence::create(CCDelayTime::create(.5f), CCEaseBounceOut::create(CCScaleTo::create(.5f, 1.5f)), nullptr));

    auto placeMenu = CCMenu::create();
    placeMenu->setPosition({0, 0});
    placeMenu->setID("place-menu");
    this->addChild(placeMenu);

    auto placeBtnSpr = ButtonSprite::create("Place");
    placeBtnSpr->setScale(.75f);
    auto placeBtn = CCMenuItemSpriteExtra::create(
        placeBtnSpr,
        this,
        menu_selector(NewCatsLayer::onPlace)
    );
    placeBtn->setID("place-btn");
    placeBtn->setPosition({winSize.width / 2, winSize.height * 0.1f});
    placeBtn->setScaleY(0);
    placeMenu->addChild(placeBtn);

    placeBtn->runAction(CCSequence::create(CCDelayTime::create(2.5f), CCEaseBackOut::create(CCScaleTo::create(.5f, 1)), nullptr));

    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    this->setKeyboardEnabled(true);
    this->setTouchEnabled(true);

    return true;
}


void NewCatsLayer::onPlace(CCObject*){
    this->removeMeAndCleanup();
}