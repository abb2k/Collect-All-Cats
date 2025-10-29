#include "CatWanderState.hpp"

#include <utils/CatUtils.hpp>

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

    target->setPosition(target->getPosition() + moveDir * moveSpeed * dt);
}

void CatWanderState::onStateStart(){
    moveDir.x = CatUtils::GetRandomFloat(-1.5f, 1.5f);
    moveDir.y = CatUtils::GetRandomFloat(-1.5f, 1.5f);

    moveSpeed = CatUtils::GetRandomFloat(minMaxSpeed.x, minMaxSpeed.y);
    duration = CatUtils::GetRandomFloat(minMaxDuration.x, minMaxDuration.y);

    target->getVisualParent()->setRotationY(moveDir.x < 0 ? 0 : 180);
}