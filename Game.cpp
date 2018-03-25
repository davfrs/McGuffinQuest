#include "Game.h"

bool torchLogic(Game& game) {
    COORDINATE3 location = game.map.playerLocation();
    Map map = game.map;
    bool activated = false;

    COORDINATE3 t = location.incrementX();
    if (t.isValid()) {
        if (map.isSquareRevealed(t)) {
            map.revealSquare_Coord3(t);
            activated = true;
        }
    }

    t = location.decrementX();
    if (t.isValid()) {
        if (map.isSquareRevealed(t)) {
            map.revealSquare_Coord3(t);
            activated = true;
        }
    }

    t = location.incrementY();
    if (t.isValid()) {
        if (map.isSquareRevealed(t)) {
            map.revealSquare_Coord3(t);
            activated = true;
        }
    }

    t = location.decrementY();
    if (t.isValid()) {
        if (map.isSquareRevealed(t)) {
            map.revealSquare_Coord3(t);
            activated = true;
        }
    }

    return activated;
}

Game::Game(std::string playerName, EntityStats playerStats)
        : player(playerName, playerStats), map() {

    enemyNames.push_back("Slime");
    enemyNames.push_back("Goblin");
    enemyNames.push_back("Spider");
    enemyNames.push_back("Giant Rat");
    enemyNames.push_back("Rabid Dog");
    enemyNames.push_back("Harpy");
    enemyNames.push_back("Giant Ant");
    enemyNames.push_back("Bandit");

    weaponNames.push_back("rusty axe");
    weaponNames.push_back("chipped sword");
    weaponNames.push_back("antique dagger");
    weaponNames.push_back("dull claymore");
    weaponNames.push_back("broken mace");
    weaponNames.push_back("slime's bone");

    armorNames.push_back("old chainmail");
    armorNames.push_back("beautiful cloth");

    consumables.emplace(TORCH_NAME, std::shared_ptr<Inventory::ConsumableItem>(new Inventory::ConsumableItem_Lambda([this]() { return torchLogic(*this); }, TORCH_NAME, TORCH_DESC, TORCH_WORTH)));
    consumables.emplace(POTION_NAME, std::shared_ptr<Inventory::ConsumableItem>(new Inventory::ConsumableItem_Lambda([this]() { return player.heal(POTION_HEAL); }, POTION_NAME, POTION_DESC, POTION_WORTH)));
    consumables.emplace(MAXPOTION_NAME, std::shared_ptr<Inventory::ConsumableItem>(new Inventory::ConsumableItem_Lambda([this]() { return player.heal(player.getMaxHP()); }, MAXPOTION_NAME, MAXPOTION_DESC, MAXPOTION_WORTH)));
}

std::shared_ptr<Inventory::ConsumableItem> Game::getConsumableItem(const std::string name) {
    auto potionLoc = this->consumables.find(POTION_NAME);
    if (potionLoc != this->consumables.end())
        return potionLoc->second->clone();
    return std::shared_ptr<Inventory::ConsumableItem>();
}

std::shared_ptr<Entity> Game::generateRandomEnemy(int floorLevel) {
    EntityStats baseStats;
    std::string name = this->enemyNames[rand() % this->enemyNames.size()];
    //yes, I did just randomly come up with these equations.
    baseStats.HP = (rand() % (5 + 2 * floorLevel) + floorLevel) * 5 + 25;
    baseStats.defense = (rand() % (4 + floorLevel)) / 3 + 2 + floorLevel;
    baseStats.strength = (rand() % (8 + floorLevel)) / 3 + 5 + floorLevel;

    std::shared_ptr<Entity> enemy(new Entity(name, baseStats));
    Inventory::InventoryData& inv = enemy->getInv();

    inv.addMoney((rand() % (10 * floorLevel + 20)) + 5 * floorLevel + 20);

    int p = rand() % (2 * floorLevel + 3) - floorLevel;
    if (p != 0) {
        std::string weaponName = this->weaponNames[rand() % this->weaponNames.size()];
        bool equipped = (rand() % (floorLevel+1)) == 0;

        if (p < 0) {
            p = -p;
        }
        p += floorLevel / 3;
        int value = (p - 1) * p;
        if (value == 0)
            value = p;
        value *= 50;
        auto weapon = std::shared_ptr<Inventory::Item>(new Inventory::WeaponItem(weaponName, p, value));
        inv.addIfPossible(weapon);
        if (!equipped)
            inv.unequipCurrentWeapon();
    }

    p = rand() % (2 * floorLevel + 3) - floorLevel;
    if (p != 0) {
        std::string armorName = this->armorNames[rand() % this->armorNames.size()];
        bool equipped = (rand() % (floorLevel + 1)) == 0;

        if (p < 0) {
            p = -p;
            equipped = false;
        }
        p += floorLevel / 3;
        int value = (p - 1) * p;
        if (value == 0)
            value = p;
        value *= 75;
        auto armor = std::shared_ptr<Inventory::Item>(new Inventory::ArmorItem(armorName, p, value));
        inv.addIfPossible(armor);
        if (!equipped)
            inv.unequipCurrentArmor();
    }

    return enemy;
}