#include "CatAIStateBase.hpp"

CatAIStateBase::CatAIStateBase(){
    this->autorelease();
}

void CatAIStateBase::setTargetCat(Cat* target){
    this->target = target;
}

void CatAIStateBase::update(float dt){
    
}

void CatAIStateBase::onStateStart(){

}
void CatAIStateBase::onStateEnd(){

}