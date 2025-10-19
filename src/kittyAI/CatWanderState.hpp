#pragma once

#include <kittyAI/CatAIStateBase.hpp>

class CatWanderState : public CatAIStateBase {
    public:
        static std::shared_ptr<CatWanderState> create(CCPoint minMaxDuration, CCPoint minMaxSpeed);
    private:
        void step(float dt) override;
        void onStateStart() override;

        //true = left, false = right
        bool moveDir;
        float moveSpeed;
        float duration;

        CCPoint minMaxDuration;
        CCPoint minMaxSpeed;
};