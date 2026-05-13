#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BankruptPopup : public Popup{
    public:
        static BankruptPopup* create();

        virtual void show() override;

    private:
        virtual bool init() override;

        virtual void onClose(CCObject*) override;

        void onEnter() override;

        void onExit() override;
};
