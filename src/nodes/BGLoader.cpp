#include "BGLoader.hpp"

BGLoader* BGLoader::create(float width, int BGID) {
    auto ret = new BGLoader();
    if (ret->init(width, BGID)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BGLoader::init(float width, int BGID) {
    if (!CCNode::init()) return false;

    this->width = width;
    this->BGID = BGID;


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

void BGLoader::setBackground(int BGID){
    this->removeAllChildrenWithCleanup(true);

    this->BGID = BGID;

    overallSize = 0;

    while (overallSize < width)
    {
        createBG();
    }
}

void BGLoader::createBG(){
    std::string BGName;
    if (BGID < 10)
        BGName = fmt::format("game_bg_0{}_001.png", BGID);
    else
        BGName = fmt::format("game_bg_{}_001.png", BGID);
    auto BG = CCSprite::create(BGName.c_str());
    BG->setColor({ 19, 120, 179 });
    BG->setPositionX(overallSize);
    BG->setAnchorPoint({0, 0});
    BG->setColor(color);
    this->addChild(BG);
    overallSize += BG->getContentWidth();
    toRecolor.push_back(BG);
}
