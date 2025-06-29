#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include "../nodes/Cat.hpp"
#include "../types/CatStats.hpp"

class Save{
    private:
        static std::filesystem::path savesPath;

        static std::string getCatSaveFileName(GJGameLevel* relatedLevel);

    public:
        static Result<> saveCat(Cat* cat);
        static Result<> saveCat(CatStats* stats);

        static Result<CatStats> loadCat(GJGameLevel* relatedLevel);

        static std::vector<int> getPlacedCats();
        static void setPlacedCats(const std::vector<int>& catIDs);
        static void addPlacedCat(int catID);
        static void removePlacedCats(int catID);
};