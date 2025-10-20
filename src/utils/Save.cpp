#include "Save.hpp"

#include <types/CatStatsSerializer.hpp>

std::filesystem::path Save::savesPath = Mod::get()->getSaveDir() / "cats";

Result<> Save::saveCat(Cat* cat){
    auto stats = cat->getStats();
    return saveCat(&stats);
}

Result<> Save::saveCat(CatStats* stats){
    if (!std::filesystem::exists(savesPath))
    GEODE_UNWRAP(file::createDirectory(savesPath));

    std::filesystem::path mySavePath = savesPath / getCatSaveFileName(stats->getLevel());

    if (!std::filesystem::exists(mySavePath)) {
        std::ofstream saveFile(mySavePath);
        saveFile.close();
    }

    auto json = matjson::Value(*stats).dump(matjson::NO_INDENTATION);
    GEODE_UNWRAP(file::writeString(mySavePath, json));

    return Ok();
}

Result<CatStats> Save::loadCat(GJGameLevel* relatedLevel){

    std::filesystem::path mySavePath = savesPath / getCatSaveFileName(relatedLevel);
    
    GEODE_UNWRAP_INTO(auto json, file::readJson(mySavePath));
    GEODE_UNWRAP_INTO(auto stats, CatStats::createFromJsonWithLevel(json, relatedLevel));
    
    return Ok(stats);
}

std::string Save::getCatSaveFileName(GJGameLevel* relatedLevel){
    return fmt::format("{}.json", relatedLevel->m_levelID.value());
}

std::vector<int> Save::getPlacedCats(){
    return Mod::get()->getSavedValue<std::vector<int>>("placedCats", {});
}
void Save::setPlacedCats(const std::vector<int>& catIDs){
    Mod::get()->setSavedValue("placedCats", catIDs);
}
void Save::addPlacedCat(int catID){
    auto placedCats = getPlacedCats();

    for (const auto& catIDCheck : placedCats)
        if (catIDCheck == catID) return;

    placedCats.push_back(catID);

    setPlacedCats(placedCats);
}
void Save::removePlacedCats(int catID){
    auto placedCats = getPlacedCats();

    bool didFindCat = false;
    for (int i = 0; i < placedCats.size(); i++)
    {
        if (placedCats[i] != catID) continue;
        
        placedCats.erase(placedCats.begin() + i);
        didFindCat = true;
        break;
    }

    if (didFindCat) setPlacedCats(placedCats);
}


void Save::saveBackground(int BGID){
    Mod::get()->setSavedValue("Background", BGID);
}

int Save::getBackground(){
    return Mod::get()->getSavedValue<int>("Background", 1);
}


void Save::saveGround(int GroundID){
    Mod::get()->setSavedValue("Ground", GroundID);
}

int Save::getGround(){
    return Mod::get()->getSavedValue<int>("Ground", 1);
}

void Save::saveBackgroundColor(ccColor3B color){
    Mod::get()->setSavedValue("BackgroundColor", color);
}

ccColor3B Save::getBackgroundColor(){
    return Mod::get()->getSavedValue<ccColor3B>("BackgroundColor", { 40, 125, 255 });
}


void Save::saveGroundColor(ccColor3B color){
    Mod::get()->setSavedValue("GroundColor", color);
}

ccColor3B Save::getGroundColor(){
    return Mod::get()->getSavedValue<ccColor3B>("GroundColor", { 0, 102, 255 });
}

void Save::saveGroundOutlineColor(ccColor3B color){
    Mod::get()->setSavedValue("GroundColor", color);
}

ccColor3B Save::getGroundOutlineColor(){
    return Mod::get()->getSavedValue<ccColor3B>("GroundColor", { 0, 102, 255 });
}


bool Save::doesExistCatForLevel(GJGameLevel* relatedLevel){
    std::filesystem::path mySavePath = savesPath / getCatSaveFileName(relatedLevel);

    return std::filesystem::exists(mySavePath);
}