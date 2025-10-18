#include "CatWanderState.hpp"

#include <utils/Utils.hpp>

std::shared_ptr<CatWanderState> CatWanderState::create(){
    return std::make_shared<CatWanderState>();
}

void CatWanderState::step(float dt){
    duration -= dt;

    //log::info("duration: {} | movement speed: {} | dir: {}", duration, moveSpeed, moveDir);

    if (duration <= 0) endState();

    target->setPositionX(target->getPositionX() + moveSpeed * (moveDir ? -1 : 1) * dt);
}

void CatWanderState::onStateStart(){
    moveDir = Utils::GetRandomInt(0, 1) == 0 ? true : false;

    moveSpeed = Utils::GetRandomFloat(1, 5);
    duration = Utils::GetRandomFloat(1, 3);
}