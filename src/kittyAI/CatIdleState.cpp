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
}