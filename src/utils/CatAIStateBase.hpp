#pragma once

#include "../nodes/Cat.hpp"

class CatAIStateBase : public CCObject{
    public:
        CatAIStateBase();
    
        void setTargetCat(Cat* target);

        virtual void update(float dt);

        virtual void onStateStart();
        virtual void onStateEnd();
    protected:
        
    private:

        Cat* target = nullptr;
};