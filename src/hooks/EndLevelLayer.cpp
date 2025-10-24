#include "EndLevelLayer.hpp"

void CACEndLevelLayer::customSetup(){
    EndLevelLayer::customSetup();
    log::info("TS WORKING!!");

    m_fields->catContainer = CCNode::create();
    m_fields->catContainer->setID("cat-conatiner");
    m_fields->catContainer->setPosition({165, 160});
    m_mainLayer->addChild(m_fields->catContainer);
    
    
    auto obtainCatLabel = CCLabelBMFont::create("+1", "bigFont.fnt");
    obtainCatLabel->setScale(.7f);
    obtainCatLabel->setPositionX(-obtainCatLabel->getScaledContentWidth() / 2);
    obtainCatLabel->setOpacity(0);
    m_fields->catContainer->addChild(obtainCatLabel);

    auto obtainCatSpr = CCSprite::createWithSpriteFrameName("default_cat.png"_spr);
    obtainCatSpr->setScale(.25f);
    obtainCatSpr->setPositionX(obtainCatSpr->getScaledContentWidth() / 2);
    obtainCatSpr->setOpacity(0);
    m_fields->catContainer->addChild(obtainCatSpr);

}

void CACEndLevelLayer::catEnter(){
    for (const auto& child : CCArrayExt<CCNode*>(m_fields->catContainer->getChildren()))
    {
        child->runAction(CCFadeTo::create(.15f, 255));
    }

    m_fields->catContainer->setScale(3);
    m_fields->catContainer->runAction(CCEaseBounceOut::create(CCScaleTo::create(.3f, 1)));
    m_fields->catContainer->runAction(CCSequence::create(
            CCDelayTime::create(.15f), 
            CCCallFunc::create(this, callfunc_selector(CACEndLevelLayer::spawnCircleWave)),
            nullptr
        ));
    
}

void CACEndLevelLayer::playEndEffect(){
    EndLevelLayer::playEndEffect();

    if (m_playLayer == nullptr) return;
    if (m_playLayer->m_level == nullptr) return;
    
    auto level = m_playLayer->m_level;
    if (level->m_demonDifficulty == 6 && level->m_demon.value() == 1)
        catEnter();
    log::info("CACEndLevelLayer::playEndEffect");
}

void CACEndLevelLayer::spawnCircleWave(){
    auto circleAnim = CCCircleWave::create(25, 80, .3f, false);
    circleAnim->m_color = { 255, 70, 70 };
    circleAnim->m_opacity = .7f;
    FMODAudioEngine::sharedEngine()->playEffect("meow.mp3"_spr, 1.5f, 1, 1);
    m_fields->catContainer->addChild(circleAnim);
}
