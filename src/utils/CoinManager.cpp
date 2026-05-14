#include "CoinManager.hpp"

#include <utils/Save.hpp>
#include <types/CosmeticMetadataSerializer.hpp>
#include <utils/BuyItemEvent.hpp>

int CoinManager::COINS_PER_LVL = 150;

int CoinManager::getCoinCount(){
    auto completedLevels = GameLevelManager::get()->getCompletedLevels(true);

    std::vector<GJGameLevel*> beaten{};

    for (const auto& level : CCArrayExt<GJGameLevel*>(completedLevels))
    {
        if (level->m_demonDifficulty == 6 && level->m_demon.value() == 1) beaten.push_back(level);
    }

    if (!beaten.size()) return 0;

    int coins = 0;

    for (const auto& level : beaten)
        coins += CoinManager::COINS_PER_LVL;

    auto accessories = Save::getUnlockedAccessories();

    for (const auto& accessory : accessories)
    {
        auto metaLoc = Mod::get()->getResourcesDir() / fmt::format("{}.json", accessory);
        if (!std::filesystem::exists(metaLoc)){
            continue;
        }

        auto readRes = file::readFromJson<CosmeticMetadata>(metaLoc);
        if (readRes.isErr()){
            continue;
        }
        
        coins -= readRes.unwrap().cost;
    }

    return coins;

}

void CoinManager::buyItem(const std::string& itemCategory, const std::string& itemID){
    auto unlocks = Save::getUnlockedAccessories();

    auto toInsert = fmt::format("{}-{}", itemCategory, itemID);

    if (std::find(unlocks.begin(), unlocks.end(), toInsert) != unlocks.end()) return;

    unlocks.push_back(toInsert);

    Save::saveUnlockedAccessories(unlocks);

    BuyItemEvent().send(getCoinCount());
}

void CoinManager::sellItem(const std::string& itemCategory, const std::string& itemID){
    auto unlocks = Save::getUnlockedAccessories();

    auto toInsert = fmt::format("{}-{}", itemCategory, itemID);

    auto find = std::find(unlocks.begin(), unlocks.end(), toInsert);

    if (find == unlocks.end()) return;

    unlocks.erase(find);

    Save::saveUnlockedAccessories(unlocks);

    BuyItemEvent().send(getCoinCount());
}