#include "CatIdleState.hpp"

#include <utils/CatUtils.hpp>

std::shared_ptr<CatIdleState> CatIdleState::create(CCPoint minMaxDuration){
    auto ret = std::make_shared<CatIdleState>();
    ret->minMaxDuration = minMaxDuration;
    return ret;
}

void CatIdleState::step(float dt){
    duration -= dt;

    if (duration <= 0) endState();
}

void CatIdleState::onStateStart(){
    duration = CatUtils::GetRandomFloat(minMaxDuration.x, minMaxDuration.y);
    auto model = target->getVisualParent()->getAssetForCategory("cat")->getModel();

    if (model){
        model->playAnimation("wave", .15f);
        model->setAnimSpeedMultiplier(1);
    }
}