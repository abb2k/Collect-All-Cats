#include "CatAIStateBase.hpp"

void CatAIStateBase::setTargetCat(Cat* target){
    this->target = target;
}

void CatAIStateBase::endState(){
    if (!isOngoing) return;
    isOngoing = false;
    
    onStateEnd();

    onStateEnded(this);
}

void CatAIStateBase::startState(){
    if (isOngoing) return;
    onStateStart();

    isOngoing = true;
}

void CatAIStateBase::onStateEndedCallback(const std::function<void(CatAIStateBase*)>& callback){
    onStateEnded = callback;
}

void CatAIStateBase::update(float dt){
    if (isOngoing)
        step(dt);
}