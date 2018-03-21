#include "Inventory.h"

namespace Inventory {
	bool ConsumableItem::merge(shared_ptr<ConsumableItem> other) {
		if (this->mergeable(other)) {
			this->count += other->count;
			return true;
		}
		return false;
	}
	bool ConsumableItem::attemptUse() {
		if (_use())
			if (--this->count <= 0)
				return true;
		return false;
	}

		
	shared_ptr<ConsumableItem> ConsumableItem_Lambda::clone() {
		return (*new ConsumableItem_Lambda(this->useFunction, this->getName(), this->getDescription(), this->getBaseWorth()))();
	}
	unsigned long InventoryData::addMoney(unsigned long money) {
		unsigned long temp = this->money + money;
		if (money > temp) {
			this->money = 0;
			this->money--;
			return temp + 1;
		}
		this->money = temp;
		return 0;
	}
	unsigned long InventoryData::removeMoney(unsigned long money) {
		if (this->money < money) {
			money -= this->money;
			this->money = 0;
			return money;
		}
		this->money -= money;
		return 0;
	}
	vector<shared_ptr<Item>> InventoryData::addAsPossible(vector<shared_ptr<Item>> items) {
		vector<shared_ptr<Item>> faileds;
		for (shared_ptr<Item> each : items) {
			if (each == nullptr)
				continue;
			ItemType type = each->getType();
			switch (type) {
			case ItemType::Money:{
				shared_ptr<MoneyItem> moneyItem = dynamic_pointer_cast<MoneyItem> (each);
				unsigned long worth = moneyItem->getWorth();
				unsigned long remaining = this->addMoney(worth);
				if (remaining == 0) {

				} else {
					//shared_ptr<Item> money);
					faileds.push_back((*new MoneyItem(remaining))());
				}
				break;}
			case ItemType::StackableConsumable:{
				shared_ptr<ConsumableItem> consumableItem = dynamic_pointer_cast<ConsumableItem> (each);
				bool nadded = true;
				for (shared_ptr<ConsumableItem> e : this->consumables) {
					if (e->merge(consumableItem)) {
						nadded = false;
						break;
					}
				}
				if (nadded) {
					if (this->inventoryLimit > this->currentInventoryCount) {
						this->currentInventoryCount++;
						this->consumables.push_back(consumableItem);
					} else {
						faileds.push_back(each);
					}
				}
				break;}
			case ItemType::Weapon:{
				shared_ptr<WeaponItem> weapon = dynamic_pointer_cast<WeaponItem>(each);
				if (this->currentWeapon == nullptr) {
					this->currentWeapon = weapon;
				} else {
					if (inventoryLimit > currentInventoryCount) {
						this->currentInventoryCount++;
						this->otherWeapons.push_back(weapon);
					} else {
						faileds.push_back(each);
					}
				}
				break;}
			case ItemType::Armor:{
				shared_ptr<ArmorItem> armor = dynamic_pointer_cast<ArmorItem>(each);
				if (this->currentArmor == nullptr) {
					this->currentArmor = armor;
				} else {
					if (inventoryLimit > currentInventoryCount) {
						this->currentInventoryCount++;
						this->otherArmors.push_back(armor);
					} else {
						faileds.push_back(each);
					}
				}
				break;}
			}
		}
		return faileds;
	}
	bool InventoryData::swapActiveArmor(int armorNumber) {
		if (this->otherArmors.size() <= (unsigned)armorNumber || armorNumber < 0)
			return false;
		if (this->currentArmor == nullptr) {
			this->currentArmor = this->otherArmors[armorNumber];
			this->otherArmors.erase(this->otherArmors.begin() + armorNumber);
			this->currentInventoryCount--;
		} else {
			this->otherArmors[armorNumber].swap(this->currentArmor);
		}
		return true;
	}
	bool InventoryData::swapActiveWeapon(int weaponNumber) {
		if (this->otherWeapons.size() <= (unsigned)weaponNumber || weaponNumber < 0)
			return false;
		if (this->currentWeapon == nullptr) {
			this->currentWeapon = this->otherWeapons[weaponNumber];
			this->otherWeapons.erase(this->otherWeapons.begin() + weaponNumber);
			this->currentInventoryCount--;
		}
		else {
			this->otherWeapons[weaponNumber].swap(this->currentWeapon);
		}
		return true;
	}
	AttemptedUseStates InventoryData::useConsumable(int consumableNumber) {
		if (this->consumables.size() <= (unsigned)consumableNumber || consumableNumber < 0)
			return AttemptedUseStates::DoesNotExist;
		shared_ptr<ConsumableItem> consumable = this->consumables[consumableNumber];
		if (consumable->attemptUse()) {
			if (consumable->getCount() <= 0) {
				this->consumables.erase(this->consumables.begin() + consumableNumber);
				this->currentInventoryCount--;
			}
			return AttemptedUseStates::Used;
		}
		return AttemptedUseStates::Unusable;
	}
	shared_ptr<WeaponItem> InventoryData::removeCurrentWeapon() {
		shared_ptr<WeaponItem> ret = this->currentWeapon;
		this->currentWeapon.reset();
		return ret;
	}
	shared_ptr<ArmorItem> InventoryData::removeCurrentArmor() {
		shared_ptr<ArmorItem> ret = this->currentArmor;
		this->currentWeapon.reset();
		return ret;
	}
	shared_ptr<WeaponItem> InventoryData::removeWeapon(int weaponNumber) {
		if (this->otherWeapons.size() <= (unsigned)weaponNumber || weaponNumber < 0)
			return shared_ptr<WeaponItem>();
		shared_ptr<WeaponItem> ret = this->otherWeapons[weaponNumber];
		this->otherWeapons.erase(this->otherWeapons.begin() + weaponNumber);
		this->currentInventoryCount--;
		return ret;
	}
	shared_ptr<ArmorItem> InventoryData::removeArmor(int armorNumber) {
		if (this->otherArmors.size() <= (unsigned)armorNumber || armorNumber < 0)
			return shared_ptr<ArmorItem>();
		shared_ptr<ArmorItem> ret = this->otherArmors[armorNumber];
		this->otherArmors.erase(this->otherArmors.begin() + armorNumber);
		this->currentInventoryCount--;
		return ret;
	}
	shared_ptr<ConsumableItem> InventoryData::removeConsumable(int consumableNumber) {
		if (this->consumables.size() <= (unsigned)consumableNumber || consumableNumber < 0)
			return shared_ptr<ConsumableItem>();
		shared_ptr<ConsumableItem> ret = this->consumables[consumableNumber];
		this->consumables.erase(this->consumables.begin() + consumableNumber);
		this->currentInventoryCount--;
		return ret;
	}
	void InventoryData::unequipCurrentWeapon() {
		auto t = this->currentWeapon;
		this->otherWeapons.push_back(t);
		this->currentWeapon.reset();
	}
	void InventoryData::unequipCurrentArmor() {
		auto t = this->currentArmor;
		this->otherArmors.push_back(t);
		this->currentArmor.reset();
	}

	ostream& operator<<(ostream& out, InventoryData& inv) {
		out << inv.money << ' ' << MONEYNAME << endl;

		if (inv.hasActiveWeapon()) {
			out << *inv.getCurrentWeapon();
		} else {
			out << "no weapon";
		}
		out << endl;

		if (inv.hasActiveArmor()) {
			out << *inv.getCurrentArmor();
		}
		else {
			out << "no armor";
		}
		out << endl;

		out << "inventory space: " << inv.currentInventoryCount << '/' << inv.inventoryLimit << endl;

		return out;
	}

	void InventoryData::printWeapons(ostream& out) const {
		out << "Active Weapon:" << endl;
		if (this->hasActiveWeapon()) {
			out << *this->getCurrentWeapon() << endl;
		} else {
			out << "no active weapon" << endl;
		}
		out << "Other Weapons:" << endl;
		for (shared_ptr<WeaponItem> each : this->getOtherWeapons()) {
			out << *each << endl;
		}
	}
	void InventoryData::printArmors(ostream& out) const {
		out << "Active Armor:" << endl;
		if (this->hasActiveArmor()) {
			out << *this->getCurrentArmor() << endl;
		} else {
			out << "no active armor" << endl;
		}
		out << "Other Armors:" << endl;
		for (shared_ptr<ArmorItem> each : this->getOtherArmors()) {
			out << *each << endl;
		}
	}
	void InventoryData::printConsumables(ostream& out) const {
		for (shared_ptr<ConsumableItem> each : this->getConsumables()) {
			out << *each << endl;
		}
	}
	vector<shared_ptr<Item>> InventoryData::dropEverything() {
		vector<shared_ptr<Item>> ret;
		if (this->money != 0) {
			ret.push_back((*new MoneyItem(this->money))());
			this->money = 0;
		}
		if (this->currentArmor != nullptr) {
			shared_ptr<Item> t = this->currentArmor;
			ret.push_back(t);
			this->currentArmor.reset();
		}
		if (this->currentWeapon != nullptr) {
			shared_ptr<Item> t = this->currentWeapon;
			ret.push_back(t);
			this->currentWeapon.reset();
		}
		for (shared_ptr<Item> e : this->otherWeapons) {
			shared_ptr<Item> e2 = e;
			ret.push_back(e2);
		}
		for (shared_ptr<Item> e : this->otherArmors) {
			shared_ptr<Item> e2 = e;
			ret.push_back(e2);
		}
		for (shared_ptr<Item> e : this->consumables) {
			shared_ptr<Item> e2 = e;
			ret.push_back(e2);
		}

		return ret;
	}
}