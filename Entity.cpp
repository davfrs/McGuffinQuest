#include "Entity.h"

void Entity::resetActiveStats() {
	int hp = this->active.HP;
	this->active = this->original;
	this->active.HP = hp;
}

bool Entity::attackEntity(Entity& other) {
	int damage = this->active.strength;
	if (this->inv.hasActiveWeapon())
		damage += this->inv.getCurrentWeapon()->getPower();
	
	return other.defendFromAttack(damage);
}
bool Entity::defendFromAttack(int damage) {
	damage -= this->active.defense;
	if (this->inv.hasActiveArmor())
		damage -= this->inv.getCurrentArmor()->getPower();

	return this->takeExactDamage(damage);
}
bool Entity::takeExactDamage(int damage) {
	if (damage <= 0)
		return false;
	return (this->active.HP -= damage) <= 0;
}

bool Entity::heal(int health) {
	if (health <= 0)
		return false;
	int o = this->active.HP;
	if ((this->active.HP += health) > this->original.HP)
		this->active.HP = this->original.HP;
	return o != this->active.HP;
}