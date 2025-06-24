#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BGLoader : public CCNode {
public:
    static BGLoader* create(float width, const char* sprite);

    void setBackground(const char* newSprite);

    void setColor(ccColor3B color);

private:
    virtual bool init(float width, const char* sprite);
    float width;
    const char* sprite;
    float overallSize;

    std::vector<CCSprite*> toRecolor{};
    ccColor3B color = { 255, 255, 255 };

    void createBG();
};
