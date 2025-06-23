#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CatsLayer : public CCLayer {
private:
    virtual bool init();

    virtual void keyBackClicked();

public:
    static CatsLayer* create();

    ScrollLayer* ScrollNode;

    void update(float dt);
};