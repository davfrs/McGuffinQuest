#include "Inventory.h"

namespace Inventory {
	bool ConsumableItem::merge(ConsumableItem& other) {
		if (this->mergeable(other)) {
			this->count += other.count;
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
	

	bool ConsumableItem_Lambda::mergeable(const ConsumableItem& other) {
		try {
			const ConsumableItem_Lambda& o2 = dynamic_cast<const ConsumableItem_Lambda&>(other);
			return this->useFunction.target == o2.useFunction.target;
		}
		catch (std::bad_cast& e) {
			return false;
		}
	}

	Inventory::~Inventory() {
		if (currentWeapon != nullptr)
			delete currentWeapon;
		if (currentArmor != nullptr)
			delete currentArmor;
	}
	unsigned long Inventory::addMoney(unsigned long money) {
		unsigned long temp = this->money + money;
		if (money > temp) {
			this->money = 0;
			this->money--;
			return temp + 1;
		}
		this->money = temp;
		return 0;
	}
	unsigned long Inventory::removeMoney(unsigned long money) {
		if (this->money < money) {
			money -= this->money;
			this->money = 0;
			return money;
		}
		this->money -= money;
		return 0;
	}
	vector<Item&> Inventory::addAsPossible(vector<Item&> items) {
		vector<Item&> faileds;
		for (Item& each : items) {
			ItemType type = each.getType();
			switch (type) {
			case ItemType::Money:
				MoneyItem& moneyItem = dynamic_cast<MoneyItem&> (each);
				unsigned long worth = moneyItem.getWorth();
				unsigned long remaining = this->addMoney(worth);
				if (remaining == 0) {

				} else {
					faileds.push_back(MoneyItem(remaining));
				}
				break;
			case ItemType::StackableConsumable:
				ConsumableItem& consumableItem = dynamic_cast<ConsumableItem&> (each);
				bool nadded = true;
				for (ConsumableItem& e : this->consumables) {
					if (e.merge(consumableItem)) {
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
				break;
			case ItemType::Weapon:
				WeaponItem& weapon = dynamic_cast<WeaponItem&>(each);
				if (this->currentWeapon == nullptr) {
					this->currentWeapon = new Reference<WeaponItem>(weapon);
				} else {
					if (inventoryLimit > currentInventoryCount) {
						this->currentInventoryCount++;
						this->otherWeapons.push_back(weapon);
					} else {
						faileds.push_back(each);
					}
				}
				break;
			case ItemType::Armor:
				ArmorItem& armor = dynamic_cast<ArmorItem&>(each);
				if (this->currentArmor == nullptr) {
					this->currentArmor = new Reference<ArmorItem>(armor);
				} else {
					if (inventoryLimit > currentInventoryCount) {
						this->currentInventoryCount++;
						this->otherArmors.push_back(armor);
					} else {
						faileds.push_back(each);
					}
				}
				break;
			}
		}
		return faileds;
	}
	bool Inventory::swapActiveArmor(int armorNumber) {
		if (this->otherArmors.size() <= armorNumber || armorNumber < 0)
			return false;
		if (this->currentArmor == nullptr) {
			this->currentArmor = new Reference<ArmorItem>(this->otherArmors[armorNumber]);
			this->otherArmors.erase(this->otherArmors.begin() + armorNumber);
		} else {
			ArmorItem& temp = this->otherArmors[armorNumber];
			this->otherArmors.erase(this->otherArmors.begin() + armorNumber);
			this->otherArmors.push_back(this->currentArmor->get());
			delete this->currentArmor;
			this->currentArmor = new Reference<ArmorItem>(temp);

		}
		return true;
	}
	bool Inventory::swapActiveWeapon(int weaponNumber) {
		if (this->otherWeapons.size() <= weaponNumber || weaponNumber < 0)
			return false;
		if (this->currentWeapon == nullptr) {
			this->currentWeapon = new Reference<WeaponItem>(this->otherWeapons[weaponNumber]);
			this->otherWeapons.erase(this->otherWeapons.begin() + weaponNumber);
		}
		else {
			WeaponItem& temp = this->otherWeapons[weaponNumber];
			this->otherWeapons.erase(this->otherWeapons.begin() + weaponNumber);
			this->otherWeapons.push_back(this->currentWeapon->get());
			delete this->currentWeapon;
			this->currentWeapon = new Reference<WeaponItem>(temp);
		}
		return true;
	}
	AttemptedUseStates Inventory::useConsumable(int consumableNumber) {
		if (this->consumables.size() <= consumableNumber || consumableNumber < 0)
			return AttemptedUseStates::DoesNotExist;
		if (this->consumables[consumableNumber].attemptUse()) {
			if (this->consumables[consumableNumber].getCount() <= 0) {
				this->consumables.erase(this->consumables.begin() + consumableNumber);
			}
			return AttemptedUseStates::Used;
		}
		return AttemptedUseStates::Unusable;
	}
	WeaponItem& Inventory::removeCurrentWeapon() {
		WeaponItem& ret = this->currentWeapon->get();
		delete this->currentWeapon;
		return ret;
	}
	ArmorItem& Inventory::removeCurrentArmor() {
		ArmorItem& ret = this->currentArmor->get();
		delete this->currentWeapon;
		return ret;
	}

	ostream& operator<<(ostream& out, Inventory& inv) {
		out << inv.money << ' ' << MONEYNAME << endl;

		if (inv.hasActiveWeapon()) {
			out << inv.getCurrentWeapon();
		} else {
			out << "no weapon";
		}
		out << endl;

		if (inv.hasActiveArmor()) {
			out << inv.getCurrentArmor();
		}
		else {
			out << "no armor";
		}
		out << endl;

		out << "inventory space: " << inv.currentInventoryCount << '/' << inv.inventoryLimit << endl;

		return out;
	}

	void Inventory::printWeapons(ostream& out) const {
		out << "Active Weapon:" << endl;
		if (this->hasActiveWeapon()) {
			out << this->getCurrentWeapon() << endl;
		} else {
			out << "no active weapon" << endl;
		}
		out << "Other Weapons:" << endl;
		for (WeaponItem& each : this->getOtherWeapons()) {
			out << each << endl;
		}
	}
	void Inventory::printArmors(ostream& out) const {
		out << "Active Armor:" << endl;
		if (this->hasActiveArmor()) {
			out << this->getCurrentArmor() << endl;
		} else {
			out << "no active armor" << endl;
		}
		out << "Other Armors:" << endl;
		for (ArmorItem& each : this->getOtherArmors()) {
			out << each << endl;
		}
	}
	void Inventory::printConsumables(ostream& out) const {
		for (ConsumableItem& each : this->getConsumables()) {
			out << each << endl;
		}
	}
}