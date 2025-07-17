#include "GroundLoader.hpp"

GroundLoader* GroundLoader::create(float width, int GroundID, int extraAmount) {
    auto ret = new GroundLoader();
    if (ret->init(width, GroundID, extraAmount)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GroundLoader::init(float width, int GroundID, int extraAmount) {
    if (!CCNode::init()) return false;

    this->width = width;
    this->GroundID = GroundID;
    this->extraAmount = extraAmount;

    while (overallSize < width)
    {
        createGround();
    }

    for (int i = 0; i < extraAmount; i++)
    {
        createGround();
    }
    
    return true;
}

void GroundLoader::createGround(){
    std::string groundName;
    if (GroundID < 10)
        groundName = fmt::format("groundSquare_0{}_001.png", GroundID);
    else
        groundName = fmt::format("groundSquare_{}_001.png", GroundID);
    auto floor = CCSprite::create(groundName.c_str());
    floor->setSkewX(60);
    floor->setColor({ 48, 100, 129 });
    floor->setPositionX(overallSize);
    floor->setPositionY(15.1f);
    floor->setAnchorPoint({1, .5f});
    floor->setScaleY(.75f);
    this->addChild(floor);
    floor->setColor(color);
    floorLength = floor->getContentWidth();
    grounds.push_back(floor);
    floor->setZOrder(-1);

    auto line = CCSprite::createWithSpriteFrameName("blockOutline_14new_001.png");
    line->setPosition(ccp(floor->getContentWidth() / 2 + overallSize, floor->getPositionY() +  floor->getScaledContentHeight() / 2));
    line->setSkewX(60);
    line->setZOrder(2);
    line->setScaleX(7.4f);
    this->addChild(line);
    line->setColor(outlineColor);
    overallSize += floor->getContentWidth();
    outlines.push_back(line);
}

void GroundLoader::setGround(int GroundID){

    this->removeAllChildrenWithCleanup(true);

    this->GroundID = GroundID;

    floorLength = 0;
    overallSize = 0;

    while (overallSize < width)
    {
        createGround();
    }

    for (int i = 0; i < extraAmount; i++)
    {
        createGround();
    }
}

void GroundLoader::setColor(ccColor3B color){
    for (const auto& sprite : grounds)
        sprite->setColor(color);
    
    this->color = color;
}

void GroundLoader::setOutlineColor(ccColor3B color){
    for (const auto& sprite : outlines)
        sprite->setColor(color);
    
    this->outlineColor = color;
}