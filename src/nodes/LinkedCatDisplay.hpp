#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class LinkedCatDisplay : public CCNode {
public:
    static LinkedCatDisplay* create();

private:
    virtual bool init();
};
