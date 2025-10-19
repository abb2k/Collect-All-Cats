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

    target->setPosition(target->getPosition() + moveDir * moveSpeed * dt);
}

void CatWanderState::onStateStart(){
    moveDir.x = Utils::GetRandomFloat(-1.5f, 1.5f);
    moveDir.y = Utils::GetRandomFloat(-1.5f, 1.5f);

    moveSpeed = Utils::GetRandomFloat(minMaxSpeed.x, minMaxSpeed.y);
    duration = Utils::GetRandomFloat(minMaxDuration.x, minMaxDuration.y);

    target->getVisualParent()->setRotationY(moveDir.x < 0 ? 0 : 180);
}