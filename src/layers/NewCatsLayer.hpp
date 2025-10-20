#pragma once

#include <Geode/Geode.hpp>

#include <types/CatStats.hpp>

using namespace geode::prelude;

class NewCatsLayer : public CCLayer {
public:
    static NewCatsLayer* create(const std::vector<CatStats>& newCats);

private:
    virtual bool init(const std::vector<CatStats>& newCats);

    CCAnimation* lighningAnim;

    void onPlace(CCObject*);
    void allowLeave();

    void keyBackClicked() override;
    void registerWithTouchDispatcher() override;

    bool canLeave;

    CCLayer* parentLayer = nullptr;

    float originalVolume;
};
