#pragma once

#include <kittyAI/CatAIStateBase.hpp>

class CatIdleState : public CatAIStateBase {
    public:
        static std::shared_ptr<CatIdleState> create(CCPoint minMaxDuration);
    private:
        void step(float dt) override;
        void onStateStart() override;

        float duration;
        CCPoint minMaxDuration;
};