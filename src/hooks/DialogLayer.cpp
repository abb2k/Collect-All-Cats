#include "DialogLayer.hpp"

void CACDialogLayer::displayDialogObject(DialogObject* dialogue){
    DialogLayer::displayDialogObject(dialogue);

    std::string cacDialogSpr = "";
    std::tuple<std::string, int, float> cacDialogSprinfo;

    for (const auto& existingIcons : m_fields->icons)
    {
        if (std::get<1>(existingIcons) == dialogue->m_characterFrame){
            cacDialogSpr = std::get<0>(existingIcons);
            cacDialogSprinfo = existingIcons;
            break;
        }
    }

    if (cacDialogSpr != "") {
        m_characterSprite->setVisible(false);

        auto icon = m_mainLayer->getChildByID("cac_portrait"_spr);
        if (icon != nullptr) icon->removeMeAndCleanup();

        auto newIcon = CCSprite::createWithSpriteFrameName(cacDialogSpr.c_str());
        if (newIcon == nullptr) newIcon = CCSprite::create(cacDialogSpr.c_str());
        if (newIcon == nullptr) return;
        newIcon->setID("cac_portrait"_spr);
        newIcon->setPosition(m_characterSprite->getPosition());
        newIcon->setZOrder(4);
        newIcon->setScale(std::get<2>(cacDialogSprinfo));
        m_mainLayer->addChild(newIcon);
    }
    else{
        auto icon = m_mainLayer->getChildByID("cac_portrait"_spr);
        if (icon != nullptr) icon->setVisible(false);
    }
}

void CACDialogLayer::useTaggedSprites(const std::vector<std::tuple<std::string, int, float>>& icons){
    m_fields->icons = icons;
}

DialogLayer* CACDialogLayer::createWithTaggedSprites(CCArray* objects, int bg, const std::vector<std::tuple<std::string, int, float>>& icons){
    objects->insertObject(DialogObject::create("", "", 0, 1, true, {0,0,0}), 0);
    auto ret = DialogLayer::createWithObjects(objects, bg);
    static_cast<CACDialogLayer*>(ret)->useTaggedSprites(icons);
    ret->displayNextObject();

    return ret;
}