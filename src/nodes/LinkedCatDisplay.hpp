#pragma once

#include <Geode/Geode.hpp>
#include <types/CatStats.hpp>

using namespace geode::prelude;

class LinkedCatDisplay : public CCMenu {
public:
    static LinkedCatDisplay* create(CatStats* catStats);

    void setCat(const CatStats& catStats);

private:
    virtual bool init(CatStats* catStats);
};
