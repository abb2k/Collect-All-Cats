#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GroundLoader : public CCNode {
public:
    static GroundLoader* create(float width, int GroundID, int extraAmount = 0);

    void setGround(int GroundID);

    void setColor(ccColor3B color);
    void setOutlineColor(ccColor3B color);

private:
    virtual bool init(float width, int GroundID, int extraAmount);

    void createGround();
    float width;
    int GroundID;
    int extraAmount;
    float overallSize;
    float floorLength;

    std::vector<CCSprite*> grounds{};
    std::vector<CCSprite*> outlines{};
    ccColor3B color = { 255, 255, 255 };
    ccColor3B outlineColor = { 255, 255, 255 };
};