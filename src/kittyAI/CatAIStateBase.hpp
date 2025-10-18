#pragma once

#include <nodes/Cat.hpp>

class CatAIStateBase : public CCObject {
    public:
        void setTargetCat(Cat* target);

        void endState();
        void startState();

        void onStateEndedCallback(const std::function<void(CatAIStateBase*)>& callback);

        void update(float dt);

        std::string name;

    protected:

        virtual void step(float dt) = 0;
        
        virtual void onStateStart() {}
        virtual void onStateEnd() {}

        Cat* target = nullptr;

    private:
        
        std::function<void(CatAIStateBase*)> onStateEnded = [](CatAIStateBase*){};
        bool isOngoing = false;
};