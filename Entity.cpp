#include "Entity.h"

void Entity::resetActiveStats() {
    int hp = this->activeStats.HP;
    this->activeStats = this->originalStats;
    this->activeStats.HP = hp;
}

bool Entity::attackEntity(Entity& other) {
    int damage = this->activeStats.strength;
    if (this->inv.hasActiveWeapon())
        damage += this->inv.getCurrentWeapon()->getPower();

    return other.defendFromAttack(damage);
}

bool Entity::defendFromAttack(int damage) {
    damage -= this->activeStats.defense;
    if (this->inv.hasActiveArmor())
        damage -= this->inv.getCurrentArmor()->getPower();

    return this->takeExactDamage(damage);
}

bool Entity::takeExactDamage(int damage) {
    if (damage <= 0)
        return false;
    return (this->activeStats.HP -= damage) <= 0;
}

bool Entity::heal(int health) {
    if (health <= 0)
        return false;
    int o = this->activeStats.HP;
    if ((this->activeStats.HP += health) > this->originalStats.HP)
        this->activeStats.HP = this->originalStats.HP;
    return o != this->activeStats.HP;
}