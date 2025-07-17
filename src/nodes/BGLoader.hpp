#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BGLoader : public CCNode {
public:
    static BGLoader* create(float width, int BGID);

    void setBackground(int BGID);

    void setColor(ccColor3B color);

private:
    virtual bool init(float width, int BGID);
    float width;
    int BGID;
    float overallSize;

    std::vector<CCSprite*> toRecolor{};
    ccColor3B color = { 255, 255, 255 };

    void createBG();
};
