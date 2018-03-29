#include "Entity.h"

void Entity::resetActiveStats() {
    int hp = this->activeStats.HP;
    this->activeStats = this->originalStats;
    this->activeStats.HP = hp;
}

std::tuple<int, int, bool> Entity::attackEntity(Entity& other) {
    int damage = this->activeStats.strength;
    if (this->inv.hasActiveWeapon())
        damage += this->inv.getCurrentWeapon()->getPower();
    std::pair<int, bool> p = other.defendFromAttack(damage);
    return std::tuple<int, int, bool>{ damage, p.first, p.second };
}

std::pair<int, bool> Entity::defendFromAttack(int damage) {
    int defense = this->activeStats.defense;
    if (this->inv.hasActiveArmor())
        defense += this->inv.getCurrentArmor()->getPower();
    damage -= defense;

    return { defense, this->takeExactDamage(damage) };
}

bool Entity::takeExactDamage(int damage) {
    if (damage <= 0)
        return false;
    if ((this->activeStats.HP -= damage) < 0)
        this->activeStats.HP = 0;
    return this->activeStats.HP == 0;
}

bool Entity::heal(int health) {
    if (health <= 0)
        return false;
    int o = this->activeStats.HP;
    if ((this->activeStats.HP += health) > this->originalStats.HP)
        this->activeStats.HP = this->originalStats.HP;
    return o != this->activeStats.HP;
}