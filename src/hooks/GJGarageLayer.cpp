#include "GJGarageLayer.hpp"

#include <layers/CatsLayer.hpp>
#include <utils/Save.hpp>
#include <hooks/DialogLayer.hpp>

bool CACGarageLayer::init(){
    if (!GJGarageLayer::init()) return false;

    auto menu = CCMenu::create();
    this->addChild(menu);

    auto entryBtnSprite = CCSprite::createWithSpriteFrameName(Save::getDoorOpened() ? "abb2k_cac_open.png"_spr : "abb2k_cac_closed.png"_spr);
    entryBtnSprite->setScale(.75f);
    m_fields->entryBtn = CCMenuItemSpriteExtra::create(
        entryBtnSprite,
        nullptr,
        this,
        menu_selector(CACGarageLayer::OnMenuBtnClicked)
    );
    m_fields->entryBtn->setPosition({254, -124});
    menu->addChild(m_fields->entryBtn);

    if (!Save::getExpFirstDialogue()){
        this->setTouchEnabled(false);
        this->setKeypadEnabled(false);
        this->runAction(CCSequence::create(CCDelayTime::create(1.5f), CCCallFunc::create(this, callfunc_selector(CACGarageLayer::startDialogue)), nullptr));

        m_fields->evilPrioStealer = CCLayer::create();
        CCTouchDispatcher::get()->addTargetedDelegate(m_fields->evilPrioStealer, -999, true);
        this->addChild(m_fields->evilPrioStealer);
    }
    
    return true;
}

void CACGarageLayer::startDialogue(){
    auto array = CCArray::create();

    array->addObject(DialogObject::create("Shopkeeper", fmt::format("Hello <cg>{}</c>!", GJAccountManager::get()->m_username), 5, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", "I'm having a small issue..", 5, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", "Some creatures decided they wanna <cr>DIG A HOLE</c> in the <cy>GARAGE WALL!</c>", 6, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", "And start living there...", 6, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", "They also built a <cy>door</c>! So i cant go myself and ask them to leave! It's <cr>locked</c>!", 5, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", "Can you please <cy>get this door open</c>.. I dont know how much longer i can stand this...", 6, 1, false, {255, 255, 255}));

    m_fields->dialogue = DialogLayer::createWithObjects(array, 1);
    m_fields->dialogue->m_delegate = this;
    CCTouchDispatcher::get()->addTargetedDelegate(m_fields->dialogue, -1000, true);
    this->addChild(m_fields->dialogue, 100);
    CCTouchDispatcher::get()->removeDelegate(m_fields->evilPrioStealer);
    m_fields->evilPrioStealer->removeMeAndCleanup();
}

void CACGarageLayer::OnMenuBtnClicked(CCObject*){
    auto scene = CCScene::create();
    auto catsLayer = CatsLayer::create();

    if (!Save::getDoorOpened()){
        auto array = CCArray::create();

        array->addObject(DialogObject::create("The Cats", "meow meow...", 999, 1, true, ccColor3B{ 255, 255, 255 }));
        array->addObject(DialogObject::create("The Cats", "meow meow meow? (who is it?)", 999, 1, true, ccColor3B{ 255, 255, 255 }));
        array->addObject(DialogObject::create("The Cats", "meow m-meow meow meow.. (want to enter we see..)", 999, 1, true, ccColor3B{ 255, 255, 255 }));
        array->addObject(DialogObject::create("The Cats", "meowmeow meoww meow, <cr>meow</c> m meow meow. (to enter this door, an <cr>Extreme Demon</c> you must beat)", 999, 1, true, ccColor3B{ 255, 255, 255 }));

        auto completedLevels = GameLevelManager::get()->getCompletedLevels(true);

        std::vector<GJGameLevel*> beatenExtremes{};

        for (const auto& level : CCArrayExt<GJGameLevel*>(completedLevels))
        {
            if (level->m_demonDifficulty == 6 && level->m_difficulty == GJDifficulty::Auto) beatenExtremes.push_back(level);
        }

        bool doSaveDialogue = false;

        if (!beatenExtremes.size())
            array->addObject(DialogObject::create("The Cats", "meow moewww, meow meow meow.. (it seems that for now, we will wait..)", 999, 1, true, ccColor3B{ 255, 255, 255 }));
        else{
            doSaveDialogue = true;

            array->addObject(DialogObject::create("The Cats", "meow, me meow? (huh, what is it?)", 999, 1, true, ccColor3B{ 255, 255, 255 }));
            array->addObject(DialogObject::create("The Cats", "mmeow meoww-w <cr>meow</c> meow (you've beaten an <cr>Extreme Demon</c> we see)", 999, 1, true, ccColor3B{ 255, 255, 255 }));

            if (beatenExtremes.size() == 1){
                array->addObject(DialogObject::create("The Cats", "<cy>1</c> you have beaten.", 999, 1, true, ccColor3B{ 255, 255, 255 }));
                array->addObject(DialogObject::create("The Cats", fmt::format("<cy>{}</c>", beatenExtremes[0]->m_levelName), 999, 1, true, ccColor3B{ 255, 255, 255 }));
                array->addObject(DialogObject::create("The Cats", "well done! the door may open..", 999, 1, true, ccColor3B{ 255, 255, 255 }));
            }
            else if (beatenExtremes.size() == 2){
                array->addObject(DialogObject::create("The Cats", "<cy>2</c> you have beaten, impressive.", 999, 1, true, ccColor3B{ 255, 255, 255 }));
                array->addObject(DialogObject::create("The Cats", fmt::format("<cy>{}</c> and <cy>{}</c>", beatenExtremes[0]->m_levelName, beatenExtremes[1]->m_levelName), 999, 1, true, ccColor3B{ 255, 255, 255 }));
                array->addObject(DialogObject::create("The Cats", "extraordinary! the door may open..", 999, 1, true, ccColor3B{ 255, 255, 255 }));
            }
            else if (beatenExtremes.size() == 3){
                array->addObject(DialogObject::create("The Cats", "<cy>3</c> you have beaten, marvelous!", 999, 1, true, ccColor3B{ 255, 255, 255 }));
                array->addObject(DialogObject::create("The Cats", fmt::format("<cy>{}</c> and <cy>{}</c> and <cy>{}</c>", beatenExtremes[0]->m_levelName, beatenExtremes[1]->m_levelName, beatenExtremes[2]->m_levelName), 999, 1, true, ccColor3B{ 255, 255, 255 }));
                array->addObject(DialogObject::create("The Cats", "an incredible feat! the door may now open..", 999, 1, true, ccColor3B{ 255, 255, 255 }));
            }
            else if (beatenExtremes.size() == 4){
                array->addObject(DialogObject::create("The Cats", "<cy>4</c> you have beaten! what a show of skill!", 999, 1, true, ccColor3B{ 255, 255, 255 }));
                array->addObject(DialogObject::create("The Cats", fmt::format("<cy>{}</c>, <cy>{}</c>, <cy>{}</c> and ALSO <cy>{}</c>!", beatenExtremes[0]->m_levelName, beatenExtremes[1]->m_levelName, beatenExtremes[2]->m_levelName, beatenExtremes[3]->m_levelName), 999, 1, true, ccColor3B{ 255, 255, 255 }));
                array->addObject(DialogObject::create("The Cats", "we are sorry to have underestimated you! the door will be opened immediately!", 999, 1, true, ccColor3B{ 255, 255, 255 }));
            }
            else{
                array->addObject(DialogObject::create("The Cats", fmt::format("you've beaten <cy>{}</c> of them! how incredible!", beatenExtremes.size()), 999, 1, true, ccColor3B{ 255, 255, 255 }));
                array->addObject(DialogObject::create("The Cats", "too many extremes to name!", 999, 1, true, ccColor3B{ 255, 255, 255 }));
                array->addObject(DialogObject::create("The Cats", "you are welcome within our hideout! the door will be opened immediately!", 999, 1, true, ccColor3B{ 255, 255, 255 }));
            }
        }

        auto dlayer = CACDialogLayer::createWithTaggedSprites(array, 1, {{"default_cat.png"_spr, 999, .65f}});
        dlayer->m_delegate = this;
        if (doSaveDialogue)
            m_fields->catsTestDialogue = dlayer;

        dlayer->setZOrder(100);
        dlayer->m_mainLayer->setScale(0);
        dlayer->m_mainLayer->runAction(CCEaseBackOut::create(CCScaleTo::create(0.5f, 1)));
        this->addChild(dlayer);
        return;
    }

    scene->addChild(catsLayer);
    
    CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
    catsLayer->createCatSettingsNode(scene);
}

void CACGarageLayer::dialogClosed(DialogLayer* dialogueLayer){
    if (m_fields->dialogue == dialogueLayer){
        CCTouchDispatcher::get()->removeDelegate(m_fields->dialogue);

        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        Save::saveExpFirstDialogue(true);
    }
    else if (m_fields->catsPostDoorDialogue == dialogueLayer){
        CCTouchDispatcher::get()->removeDelegate(m_fields->catsPostDoorDialogue);

        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        Save::saveDoorOpened(true);
    }
    else if (m_fields->catsTestDialogue == dialogueLayer){
        m_fields->evilPrioStealer = CCLayer::create();
        CCTouchDispatcher::get()->addTargetedDelegate(m_fields->evilPrioStealer, -999, true);
        this->addChild(m_fields->evilPrioStealer);

        this->setTouchEnabled(false);
        this->setKeypadEnabled(false);

        this->runAction(CCSequence::create(
            CCDelayTime::create(1),
            CCCallFunc::create(this, callfunc_selector(CACGarageLayer::openDoor)),
            CCDelayTime::create(2),
            CCCallFunc::create(this, callfunc_selector(CACGarageLayer::postDoorOpenDialogue)),
            nullptr
        ));
    }
    else{
        GJGarageLayer::dialogClosed(dialogueLayer);
    }
}

void CACGarageLayer::openDoor(){
    auto newEntryBtn = CCSprite::createWithSpriteFrameName("abb2k_cac_open.png"_spr);
    newEntryBtn->setScale(.75f);
    m_fields->entryBtn->setSprite(newEntryBtn);
    FMODAudioEngine::sharedEngine()->playEffect("dooropen.wav"_spr, 1, 1, 1);
}

void CACGarageLayer::postDoorOpenDialogue(){
    CCTouchDispatcher::get()->removeDelegate(m_fields->evilPrioStealer);
    m_fields->evilPrioStealer->removeMeAndCleanup();

    auto array = CCArray::create();

    array->addObject(DialogObject::create("Shopkeeper", "Woah! What was that loud <cr>BOOM</c> sound!", 5, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", fmt::format("Oh hi <cg>{}</c>!", GJAccountManager::get()->m_username), 5, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", "Woah you got that door open!", 5, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", "Great job!", 5, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("The Cats", "meow.. (hello..)", 999, 1, true, ccColor3B{ 255, 255, 255 }));
    array->addObject(DialogObject::create("Shopkeeper", "So YOU are the things making all this noise!", 5, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", "What even are you?", 6, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", "some sort of deadlocked monster? that was exposed to lots of radiation or something?", 6, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("The Cats", "meow.. mmeow meoww moewwwwww (we are cats.. not from these lands >:()", 999, 1, true, ccColor3B{ 255, 255, 255 }));
    array->addObject(DialogObject::create("Shopkeeper", fmt::format("Umm <cg>{}</c> do u understand what they are saying?", GJAccountManager::get()->m_username), 5, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("The Cats", "We are the cats! we come in peace!", 999, 1, true, ccColor3B{ 255, 255, 255 }));
    array->addObject(DialogObject::create("Shopkeeper", "If you come in peace then what is all this noise about..", 6, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("The Cats", "We are sorry of we disturbed you, we do not wish any harm!", 999, 1, true, ccColor3B{ 255, 255, 255 }));
    array->addObject(DialogObject::create("Shopkeeper", "Yeah yeah just dont do this again..", 6, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("Shopkeeper", fmt::format("Alright, now that that's covered, i can finally exist in peace, thank you <cg>{}</c>", GJAccountManager::get()->m_username), 5, 1, false, {255, 255, 255}));
    array->addObject(DialogObject::create("The Cats", "Thanks for helping us introduce ourselves to this local, we were having trouble doing that ourselves", 999, 1, true, ccColor3B{ 255, 255, 255 }));
    array->addObject(DialogObject::create("The Cats", "Feel free to come visit our hideout anytime! See you later :)", 999, 1, true, ccColor3B{ 255, 255, 255 }));
    
    m_fields->catsPostDoorDialogue = CACDialogLayer::createWithTaggedSprites(array, 1, {{"default_cat.png"_spr, 999, .65f}});
    m_fields->catsPostDoorDialogue->m_delegate = this;
    CCTouchDispatcher::get()->addTargetedDelegate(m_fields->catsPostDoorDialogue, -1000, true);
    this->addChild(m_fields->catsPostDoorDialogue, 100);
}