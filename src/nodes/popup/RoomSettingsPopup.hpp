#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RoomSettingsPopup : public Popup<> {
    public:
        static RoomSettingsPopup* create();

    private:
        virtual bool setup() override;
};
