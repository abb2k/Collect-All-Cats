#include "BGLoader.hpp"

BGLoader* BGLoader::create(float width, const char* sprite) {
    auto ret = new BGLoader();
    if (ret->init(width, sprite)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BGLoader::init(float width, const char* sprite) {
    if (!CCNode::init()) return false;

    this->width = width;
    this->sprite = sprite;


    while (overallSize < width)
    {
        createBG();
    }

    return true;
}

void BGLoader::setColor(ccColor3B color){
    for (const auto& sprite : toRecolor)
        sprite->setColor(color);

    this->color = color;
}

void BGLoader::setBackground(const char* newSprite){
    this->removeAllChildrenWithCleanup(true);

    this->sprite = newSprite;

    overallSize = 0;

    while (overallSize < width)
    {
        createBG();
    }
}

void BGLoader::createBG(){
    auto BG = CCSprite::create(sprite);
    BG->setColor({ 19, 120, 179 });
    BG->setPositionX(overallSize);
    BG->setAnchorPoint({0, 0});
    BG->setColor(color);
    this->addChild(BG);
    overallSize += BG->getContentWidth();
    toRecolor.push_back(BG);
}
