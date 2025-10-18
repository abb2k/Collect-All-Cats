#include "SimpleToggler.hpp"

SimpleToggler* SimpleToggler::create(CCNode* offSprite, CCNode* onSprite, float scale, bool startState){
    auto ret = new SimpleToggler();
    if (ret && ret->init(offSprite, onSprite, scale, startState)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

SimpleToggler* SimpleToggler::createWithDefaults(float scale, bool startState){
    return create(
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        scale,
        startState
    );
}

bool SimpleToggler::init(CCNode* offSprite, CCNode* onSprite, float scale, bool startState){
    if (!CCMenu::init()) return false;

    this->offSprite = offSprite;
    this->onSprite = onSprite;

    offSprite->setScale(scale);
    onSprite->setScale(scale);

    offBtn = CCMenuItemSpriteExtra::create(
        offSprite,
        this,
        menu_selector(SimpleToggler::onToggledOff)
    );
    this->addChild(offBtn);
    onBtn = CCMenuItemSpriteExtra::create(
        onSprite,
        this,
        menu_selector(SimpleToggler::onToggledOn)
    );
    this->addChild(onBtn);

    this->toggle(startState);

    ignoreAnchorPointForPosition(false);
    this->setAnchorPoint({.5f, .5f});

    return true;
}

void SimpleToggler::setCallback(const std::function<void(bool)>& callback){
    this->callback = callback;
}

void SimpleToggler::setOpacity(GLubyte opacity){
    if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(offSprite))
        rgba->setOpacity(opacity);
    
    if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(onSprite))
        rgba->setOpacity(opacity);
}

void SimpleToggler::onToggledOn(CCObject* sender){
    this->toggle(false, true);
}
void SimpleToggler::onToggledOff(CCObject* sender){
    this->toggle(true, true);
}

void SimpleToggler::toggle(bool state, bool withCallback){
    this->state = state;

    offBtn->setVisible(!this->state);
    onBtn->setVisible(this->state);

    if (this->state) this->setContentSize(onBtn->getContentSize());
    else this->setContentSize(offBtn->getContentSize());

    offBtn->setPosition(this->getContentSize() / 2);
    onBtn->setPosition(this->getContentSize() / 2);

    if (withCallback && this->callback != NULL) this->callback(this->state);
}