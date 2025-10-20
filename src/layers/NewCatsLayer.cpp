#include "NewCatsLayer.hpp"

#include <nodes/LinkedCatDisplay.hpp>
#include <utils/CCPlaySound.hpp>
#include <utils/Save.hpp>
#include <layers/CatsLayer.hpp>

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

    Loader::get()->queueInMainThread([&] {
        auto parentLayer = typeinfo_cast<CCLayer*>(this->getParent());

        this->parentLayer = parentLayer;

        parentLayer->setKeypadEnabled(false);
    });

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

    lightning->runAction(
        CCSequence::create(
            CCDelayTime::create(1),
            CCPlaySound::create("lightningCut.wav"_spr, .85f, 1, 1),
            CCAnimate::create(lighningAnim),
            CCPlaySound::create("meow.mp3"_spr, 1, 1, 1),
            nullptr
        )
    );

    kittyScroll = ScrollLayer::create({winSize.width, 100}, true, false);
    kittyScroll->setID("new-cats-container");
    kittyScroll->setPosition(winSize / 2);
    kittyScroll->setAnchorPoint({.5f, .5f});
    kittyScroll->setZOrder(1);
    kittyScroll->ignoreAnchorPointForPosition(false);
    kittyScroll->m_contentLayer->setLayout(RowLayout::create()
        ->setCrossAxisOverflow(false)
        ->setGap(15)
        //->setMinRelativeScale(false)
        ->setAutoGrowAxis(kittyScroll->getContentWidth())
    );
    this->addChild(kittyScroll);

    auto kittyMenuShadow = CCSprite::create("square.png");
    kittyMenuShadow->setID("new-cats-bg-shadow");
    kittyMenuShadow->setPosition(kittyScroll->getPosition());
    kittyMenuShadow->setScaleX(kittyScroll->getContentWidth() / kittyMenuShadow->getContentWidth());
    kittyMenuShadow->setAnchorPoint(kittyScroll->getAnchorPoint());
    kittyMenuShadow->setColor({ 34, 34, 34 });
    kittyMenuShadow->setOpacity(150);
    kittyMenuShadow->setScaleY(0);
    this->addChild(kittyMenuShadow);

    for (const auto& newCat : newCats)
    {
        auto tempCat = newCat;
        auto display = LinkedCatDisplay::create(&tempCat);
        display->setNameAboveVisible(true);
        kittyScroll->m_contentLayer->addChild(display);
    }

    kittyScroll->m_contentLayer->updateLayout();

    kittyScroll->setScaleY(0);
    kittyScroll->runAction(
        CCSequence::create(
            CCDelayTime::create(1.1f), 
            CCEaseExponentialOut::create(
                CCScaleTo::create(.3f, 1)
            ), 
            nullptr
        )
    );
    kittyMenuShadow->runAction(
        CCSequence::create(
            CCDelayTime::create(1.05f), 
            CCEaseExponentialOut::create(
                CCScaleTo::create(.3f, kittyMenuShadow->getScaleX(), kittyScroll->getContentHeight() / kittyMenuShadow->getContentHeight())
            ), 
            nullptr
        )
    );

    kittyScroll->setSkewY(20);
    kittyScroll->runAction(
        CCSequence::create(
            CCDelayTime::create(1.1f),
            CCEaseBackOut::create(
                CCSkewTo::create(.5f, 0, 0)
            ), 
            nullptr
        )
    );

    kittyScroll->setContentHeight(150);

    auto title = CCLabelBMFont::create("New Cats!", "bigFont.fnt");
    title->setID("new-cats-title");
    title->setScaleX(1.5f);
    title->setScaleY(0);
    title->setPosition({winSize.width / 2, winSize.height / 1.2f});
    this->addChild(title);

    title->runAction(
        CCSequence::create(
            CCDelayTime::create(.5f),
            CCPlaySound::create("woosh.wav"_spr, 1, 1, 1),
            CCEaseBounceOut::create(
                CCScaleTo::create(.5f, 1.5f)
            ),
            nullptr
        )
    );

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

    placeBtn->runAction(
        CCSequence::create(
            CCDelayTime::create(2.5f), 
            CCEaseBackOut::create(
                CCScaleTo::create(.5f, 1)
            ), 
            nullptr
        )
    );

    this->runAction(
        CCSequence::create(
            CCDelayTime::create(2.7f), 
            CCCallFunc::create(this, callfunc_selector(NewCatsLayer::allowLeave)), 
            nullptr
        )
    );

    auto engine = FMODAudioEngine::sharedEngine();
    originalVolume = engine->m_musicVolume;
    float newVolume = std::max(originalVolume / 4, 0.01f);

    engine->m_musicVolume = newVolume;
    engine->m_backgroundMusicChannel->setVolume(newVolume);

    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    this->setKeyboardEnabled(true);
    this->setTouchEnabled(true);

    this->scheduleUpdate();

    this->newCats = newCats;

    return true;
}

void NewCatsLayer::allowLeave(){
    canLeave = true;
}


void NewCatsLayer::onPlace(CCObject*){
    keyBackClicked();
}

void NewCatsLayer::keyBackClicked(){
    if (!canLeave) return;

    if (parentLayer != nullptr) parentLayer->setKeypadEnabled(true);

    auto engine = FMODAudioEngine::sharedEngine();
    float resetVolume = std::min(originalVolume, 1.0f);
    engine->m_musicVolume = resetVolume;
    engine->m_backgroundMusicChannel->setVolume(resetVolume);

    this->removeMeAndCleanup();

    auto catsLayer = CatsLayer::activeCatLayer();
    
    for (auto& cat : newCats)
    {
        Save::addPlacedCat(cat.getLevel()->m_levelID.value());
        if (catsLayer != nullptr)
            catsLayer->addCat(cat.getLevel());
    }
}

void NewCatsLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void NewCatsLayer::update(float dt){
    kittyScroll->m_contentLayer->setScale(.75f);
    kittyScroll->m_contentLayer->setPositionY(15 + 50 / 2);
    kittyScroll->m_contentLayer->updateLayout();

    if (kittyScroll->m_contentLayer->getScaledContentWidth() > kittyScroll->getContentWidth()){
        kittyScroll->m_contentLayer->setPositionX(
            std::clamp(
                kittyScroll->m_contentLayer->getPositionX(),
                -kittyScroll->m_contentLayer->getScaledContentWidth() + kittyScroll->getContentWidth(),
                0.0f
            )
        );
    }
    else{
        kittyScroll->m_contentLayer->setPositionX((-kittyScroll->m_contentLayer->getScaledContentWidth() + kittyScroll->getContentWidth()) / 2);
    }
    
}