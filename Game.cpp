#include "Game.h"
#include <functional>

bool torchLogic(Game& game, COORDINATE3 coord) {
    if (game.map.isSquareRevealed(coord)) {
        game.map.revealSquare(coord);
        return true;
    }
    return false;
}

bool useAround(Game& game, COORDINATE3 location, std::function<bool(Game&, COORDINATE3)> useFunction) {
    Map map = game.map;
    bool activated = false;

    COORDINATE3 t = location.incrementX();
    if (t.isValid()) {
        if (useFunction(game, t)){
            activated = true;
        }
    }
    
    t = location.decrementX();
    if (t.isValid()) {
        if (useFunction(game, t)) {
            activated = true;
        }
    }

    t = location.incrementY();
    if (t.isValid()) {
        if (useFunction(game, t)) {
            activated = true;
        }
    }

    t = location.decrementY();
    if (t.isValid()) {
        if (useFunction(game, t)) {
            activated = true;
        }
    }

    return activated;
}


Game::Game(std::string playerName, EntityStats playerStats)
        : player(playerName, playerStats), map(), cheatMode(playerName == CHEATMODE_NAME){
    if (cheatMode) {
        //we don't want to go all the way to the top floor
        this->map.__setTile({ 0, 0, 0 }, GOALPOINT | UNSEEN_TILE);
    }
    enemyNames.push_back("Slime");
    enemyNames.push_back("Goblin");
    enemyNames.push_back("Spider");
    enemyNames.push_back("Giant Rat");
    enemyNames.push_back("Rabid Dog");
    enemyNames.push_back("Harpy");
    enemyNames.push_back("Giant Ant");
    enemyNames.push_back("Bandit");

    weaponNames.push_back("Rusty Axe");
    weaponNames.push_back("Chipped Sword");
    weaponNames.push_back("Antique Dagger");
    weaponNames.push_back("Dull Claymore");
    weaponNames.push_back("Broken Mace");
    weaponNames.push_back("Slime's Bone");

    armorNames.push_back("Old Chainmail");
    armorNames.push_back("Beautiful Cloth");

    consumables.emplace(TORCH_NAME, std::shared_ptr<Inventory::ConsumableItem>(new Inventory::ConsumableItem_Lambda([this]() { return useAround(*this, this->map.playerLocation(),*torchLogic); }, TORCH_NAME, TORCH_DESC, TORCH_WORTH)));
    consumables.emplace(SUPERTORCH_NAME, std::shared_ptr<Inventory::ConsumableItem>(new Inventory::ConsumableItem_Lambda([this]() { return useAround(*this, this->map.playerLocation(), [](Game& game,COORDINATE3 location) { return useAround(game, location, *torchLogic); }); }, SUPERTORCH_NAME, SUPERTORCH_DESC, SUPERTORCH_WORTH)));
    consumables.emplace(MEGATORCH_NAME, std::shared_ptr<Inventory::ConsumableItem>(new Inventory::ConsumableItem_Lambda([this]() { return useAround(*this, this->map.playerLocation(), [](Game& game, COORDINATE3 location) { return useAround(game, location, [](Game& game, COORDINATE3 location) { return useAround(game, location, *torchLogic); } ); }); }, MEGATORCH_NAME, MEGATORCH_DESC, MEGATORCH_WORTH)));
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
        bool equipped = (rand() % (floorLevel + 1)) == 0;
        if (p < 0) {
            p = -p;
        }
        p += floorLevel / 3;
        inv.addIfPossible(this->generateWeapon(p));
        if (!equipped)
            inv.unequipCurrentWeapon();
    }

    p = rand() % (2 * floorLevel + 3) - floorLevel;
    if (p != 0) {
        bool equipped = (rand() % (floorLevel + 1)) == 0;
        if (p < 0) {
            p = -p;
        }
        p += floorLevel / 3;
        inv.addIfPossible(this->generateArmor(p));
        if (!equipped)
            inv.unequipCurrentArmor();
    }

    if (this->cheatMode) {
        std::shared_ptr<Inventory::WeaponItem> cheatSword(new Inventory::WeaponItem("Excalibur", 200, 0));
        if (inv.hasActiveWeapon()) {
            inv.removeCurrentWeapon();
            inv.addIfPossible(cheatSword);
            inv.unequipCurrentWeapon();
        } else {
            //inv.addIfPossible(this->generateWeapon(1));
            inv.addIfPossible(cheatSword);
            inv.unequipCurrentWeapon();
        }
        //std::shared_ptr<Inventory::ConsumableItem> hyperTorch(new Inventory::ConsumableItem_Lambda([this]() { return useAround(*this, this->map.playerLocation(), *torchLogic); }, "hypertorch", "Reveals an entire floor!", 0));
        this->cheatMode = false;
    }
    return enemy;
}
std::shared_ptr<Inventory::ArmorItem> Game::generateArmor(int power) {
    std::string armorName = this->armorNames[rand() % this->armorNames.size()];
    int value = (power - 1) * power;
    if (value == 0)
        value = power;
    value *= 75;
    return std::shared_ptr<Inventory::ArmorItem>(new Inventory::ArmorItem(armorName, power, value));
}
std::shared_ptr<Inventory::WeaponItem> Game::generateWeapon(int power) {
    std::string weaponName = this->weaponNames[rand() % this->weaponNames.size()];
    int value = (power - 1) * power;
    if (value == 0)
        value = power;
    value *= 50;
    return std::shared_ptr<Inventory::WeaponItem>(new Inventory::WeaponItem(weaponName, power, value));
}