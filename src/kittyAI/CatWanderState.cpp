#include "CatWanderState.hpp"

#include <utils/Utils.hpp>

std::shared_ptr<CatWanderState> CatWanderState::create(CCPoint minMaxDuration, CCPoint minMaxSpeed){
    auto ret = std::make_shared<CatWanderState>();
    ret->minMaxDuration = minMaxDuration;
    ret->minMaxSpeed = minMaxSpeed;
    return ret;
}

void CatWanderState::step(float dt){
    duration -= dt;

    //log::info("duration: {} | movement speed: {} | dir: {}", duration, moveSpeed, moveDir);

    if (duration <= 0) endState();

    target->setPositionX(target->getPositionX() + moveSpeed * (moveDir ? -1 : 1) * dt);
}

void CatWanderState::onStateStart(){
    moveDir = Utils::GetRandomInt(0, 1) == 0 ? true : false;

    moveSpeed = Utils::GetRandomFloat(minMaxSpeed.x, minMaxSpeed.y);
    duration = Utils::GetRandomFloat(minMaxDuration.x, minMaxDuration.y);

    target->getVisualParent()->setRotationY(moveDir ? 0 : 180);
}