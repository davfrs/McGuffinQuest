#ifndef ENTITY
#define ENTITY

#include <string>
#include "Inventory/Inventory.h"
#include "Map/Map.h"

#define STARTING_INVENTORY_CAPACITY 8

struct EntityStats {
    int HP;
    int strength;
    int defense;
};

class Entity {
    std::string name;
    EntityStats originalStats;
    EntityStats activeStats;
    Inventory::InventoryData inv;

    bool takeExactDamage(int damage);

protected:
    std::pair<int, bool> defendFromAttack(int damage);

public:
    Entity(std::string name, EntityStats stats)
            : name(name), originalStats(stats), activeStats(stats),
              inv(STARTING_INVENTORY_CAPACITY) {
    }

    void resetActiveStats();

    std::tuple<int, int, bool> attackEntity(Entity& other);

    const std::string getName() const {
        return this->name;
    }

    Inventory::InventoryData& getInv() {
        return this->inv;
    }

    int getCurrentHP() const {
        return this->activeStats.HP;
    }

    int getMaxHP() const {
        return this->originalStats.HP;
    }

    bool heal(int health);
};

#endif
