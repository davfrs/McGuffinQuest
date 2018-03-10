#pragma once

#ifndef INVENTORY
#define INVENTORY

#include <string>
#include <vector>
#include <functional>

namespace Inventory {
	const string MONEYNAME = "gold";
	using namespace std;

	enum ItemType {
		Money,
		StackableConsumable,
		Weapon,
		Armor
	};
	class Item {
		const string displayName;
		const ItemType type;
	protected:
		Item(ItemType type, string displayName) :type(type), displayName(displayName) {}
	public:
		Item(const Item&) = delete;
		virtual ~Item() {};
		inline ItemType getType() const { return type; }
		inline const string getName() const { return displayName; }
		friend virtual ostream& operator<<(ostream& out, const Item& item) { return out << item.displayName; }
	};
	
	class MoneyItem : public Item {
		unsigned long worth;
	public:
		MoneyItem(unsigned long worth) : Item(ItemType::Money, MONEYNAME), worth(worth) {}
		inline unsigned long getWorth() const { return worth; }
		inline bool isWorthless() { return worth == 0; }
		void reassignWorth(unsigned long worth) { this->worth = worth; }
		friend virtual ostream& operator<<(ostream& out, MoneyItem& item) { return out << item.getWorth()<< ' ' <<item.getName(); }
	};
	class WeaponItem : public Item {
	public:
		WeaponItem(string name) :Item(ItemType::Weapon, name) { }
	};
	class ArmorItem : public Item {
	public:
		ArmorItem(string name) :Item(ItemType::Armor, name) { }
	};
	class ConsumableItem : public Item {
		int count;
	public:
		ConsumableItem(string name) : ConsumableItem(name, 1) {}
		ConsumableItem(string name, int count) :Item(ItemType::StackableConsumable, name), count(count) { }
		/**Returns true if the two ConsumableItem(s) can (and have been) merge.
		 * If the two are merged, the Consumable 'this' contains the merged items.
		 */
		bool merge(ConsumableItem& other);
		virtual bool mergeable(const ConsumableItem& other) = 0;
		inline int getCount() const { return count; }
		virtual bool _use() {}
		bool attemptUse();
		friend virtual ostream& operator<<(ostream& out, ConsumableItem& item) { return out << item.getName() << " ("+item.count+')'; }
	};
	class ConsumableItem_Lambda :public ConsumableItem {
		function<bool()> useFunction;
	public:
		ConsumableItem_Lambda(function<bool()> useFunction, string name) :ConsumableItem(name), useFunction(useFunction) { }
		ConsumableItem_Lambda(function<bool()> useFunction, string name, int count) :ConsumableItem(name, count), useFunction(useFunction) { }
		virtual bool _use() { return this->useFunction(); }
		virtual bool mergeable(const ConsumableItem& other);
	};
	template<typename T>
	class Reference {
		T& v;
		Reference(const Reference& copy) = delete;
	public:
		Reference(T& v) :v(v) {}
		~Reference() {}
		T& get() { return this->v; }
	};
	enum AttemptedUseStates {
		Used,
		Unusable,
		DoesNotExist
	};
	class Inventory {
		unsigned long money;
		Reference<WeaponItem>* currentWeapon = nullptr;
		Reference<ArmorItem>* currentArmor = nullptr;

		vector<ConsumableItem&> consumables;
		vector<WeaponItem&> otherWeapons;
		vector<ArmorItem&> otherArmors;
		int inventoryLimit;
		int currentInventoryCount;
	public:
		Inventory(int inventoryLimit) : inventoryLimit(inventoryLimit), currentInventoryCount(0){ }
		~Inventory();
		inline unsigned long getHeldMoney() { return money; }
		/**Returns the amount of money that was unable to be added to the inventory.
		 */
		unsigned long addMoney(unsigned long money);
		/**Returns the amount of money that was not possible to remove form the inventory.
		 */
		unsigned long removeMoney(unsigned long money);
		vector<Item&> addAsPossible(vector<Item&> items);
		bool swapActiveArmor(int armorNumber);
		bool swapActiveWeapon(int weaponNumber);
		AttemptedUseStates useConsumable(int consumableNumber);
		
		const vector<ConsumableItem&> getConsumables()const { return this->consumables; }
		const vector<WeaponItem&> getOtherWeapons()const { return this->otherWeapons; }
		const vector<ArmorItem&> getOtherArmors()const { return this->otherArmors; }
		bool  hasActiveWeapon()const { return this->currentWeapon != nullptr; }
		const WeaponItem& getCurrentWeapon()const { return this->currentWeapon->get(); }
		WeaponItem& removeCurrentWeapon();
		bool hasActiveArmor()const { return this->currentArmor != nullptr; }
		const ArmorItem& getCurrentArmor()const { return this->currentArmor->get(); }
		ArmorItem& removeCurrentArmor();

		friend ostream& operator<<(ostream& out, Inventory& inv);
		void printWeapons(ostream& out) const;
		void printArmors(ostream& out) const;
		void printConsumables(ostream& out) const;
	};
}
#endif