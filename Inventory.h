//#pragma once

#ifndef INVENTORY
#define INVENTORY

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace Inventory
{
	using namespace std;
	const string MONEYNAME("gold");

	enum ItemType {
		Money,
		StackableConsumable,
		Weapon,
		Armor
	};
	class Item {
		const string displayName;
		const ItemType type;
		const int baseWorth;
	protected:
		Item(ItemType type, string displayName, int baseWorth) :type(type), displayName(displayName), baseWorth(baseWorth) {}
	public:
		virtual ~Item() {};
		inline ItemType getType() const { return type; }
		inline const string getName() const { return displayName; }
		friend ostream& operator<<(ostream& out, const Item& item) { return item.print(out); }
		virtual ostream& print(ostream& out) const { return out << this->displayName; }
		shared_ptr<Item> operator()() { return shared_ptr<Item>(this); }
		int getBaseWorth() const { return this->baseWorth; }
	};
	
	class MoneyItem : public Item {
		unsigned long worth;
	public:
		MoneyItem(unsigned long worth) : Item(ItemType::Money, MONEYNAME, 1), worth(worth) {}
		inline unsigned long getWorth() const { return worth; }
		inline bool isWorthless() { return worth == 0; }
		void reassignWorth(unsigned long worth) { this->worth = worth; }
		virtual ostream& print(ostream& out) const { return out << this->getWorth() << ' ' << this->getName(); }
	};
	class WeaponItem : public Item {
		int power;
	public:
		WeaponItem(string name, int power, int worth) : Item(ItemType::Weapon, name, worth), power(power) { }
		int getPower() { return this->power; }
	};
	class ArmorItem : public Item {
		int power;
	public:
		ArmorItem(string name, int power, int worth) : Item(ItemType::Armor, name, worth), power(power) { }
		int getPower() { return this->power; }
	};
	class ConsumableItem : public Item {
		int count;
		const string desc;
	public:
		ConsumableItem(string name, string desc, int worth, int count =1) :Item(ItemType::StackableConsumable, name, worth), desc(desc), count(count) { }
		/**Returns true if the two ConsumableItem(s) can (and have been) merge.
		 * If the two are merged, the Consumable 'this' contains the merged items.
		 */
		bool merge(shared_ptr<ConsumableItem> other);
		void setUses(int uses) { this->count = uses; }
		virtual bool mergeable(const shared_ptr<ConsumableItem> other) { return this->getName() == other->getName(); }
		inline int getCount() const { return count; }
		virtual bool _use() = 0;
		bool attemptUse();
		ostream& print(ostream& out) const { return out << this->getName() << " (" << this->count << ')'; }
		virtual shared_ptr<ConsumableItem> clone() =0;
		shared_ptr<ConsumableItem> operator()(){ return shared_ptr<ConsumableItem>(this); }
		const string getDescription() const { return this->desc; }
	};
	class ConsumableItem_Lambda :public ConsumableItem {
		function<bool()> useFunction;
	public:
		ConsumableItem_Lambda(function<bool()> useFunction, string name, string desc, int worth) :ConsumableItem(name, desc, worth), useFunction(useFunction) { }
		ConsumableItem_Lambda(function<bool()> useFunction, string name, string desc, int worth, int count) :ConsumableItem(name, desc, worth, count), useFunction(useFunction) { }
		virtual bool _use() { return this->useFunction(); }
		virtual shared_ptr<ConsumableItem> clone();
	};
	enum AttemptedUseStates {
		Used,
		Unusable,
		DoesNotExist
	};
	class InventoryData {
		unsigned long money;
		shared_ptr<WeaponItem> currentWeapon;
		shared_ptr<ArmorItem> currentArmor;

		vector<shared_ptr<ConsumableItem>> consumables;
		vector<shared_ptr<WeaponItem>> otherWeapons;
		vector<shared_ptr<ArmorItem>> otherArmors;
		int inventoryLimit;
		int currentInventoryCount;
	public:
		InventoryData(int inventoryLimit) : inventoryLimit(inventoryLimit), currentInventoryCount(0), money(0){ }
		~InventoryData() {}
		inline unsigned long getHeldMoney() { return money; }
		/**Returns the amount of money that was unable to be added to the inventory.
		 */
		unsigned long addMoney(unsigned long money);
		/**Returns the amount of money that was not possible to remove form the inventory.
		 */
		unsigned long removeMoney(unsigned long money);
		vector<shared_ptr<Item>> addAsPossible(vector<shared_ptr<Item>> items);
		bool swapActiveArmor(int armorNumber);
		bool swapActiveWeapon(int weaponNumber);
		AttemptedUseStates useConsumable(int consumableNumber);
		
		const vector<shared_ptr<ConsumableItem>> getConsumables()const { return this->consumables; }
		const vector<shared_ptr<WeaponItem>> getOtherWeapons()const { return this->otherWeapons; }
		const vector<shared_ptr<ArmorItem>> getOtherArmors()const { return this->otherArmors; }

		bool  hasActiveWeapon()const { return this->currentWeapon != nullptr; }
		const shared_ptr<WeaponItem> getCurrentWeapon()const { return this->currentWeapon; }
		shared_ptr<WeaponItem> removeCurrentWeapon();
		void unequipCurrentWeapon();
		shared_ptr<WeaponItem> removeWeapon(int number);

		bool hasActiveArmor()const { return this->currentArmor != nullptr; }
		const shared_ptr<ArmorItem> getCurrentArmor()const { return this->currentArmor; }
		shared_ptr<ArmorItem> removeCurrentArmor();
		void unequipCurrentArmor();
		shared_ptr<ArmorItem> removeArmor(int number);

		shared_ptr<ConsumableItem> removeConsumable(int number);

		friend ostream& operator<<(ostream& out, InventoryData& inv);
		void printWeapons(ostream& out) const;
		void printArmors(ostream& out) const;
		void printConsumables(ostream& out) const;

		int getCurrentInventoryUsage() const { return this->currentInventoryCount; }
		int getInventoryCapacity() const { return this->inventoryLimit; }
		void addCapacity(int capacity) { this->inventoryLimit += capacity; }

		vector<shared_ptr<Item>> dropEverything();
		vector<shared_ptr<Item>> makeEmptyItemListVector() { return vector<shared_ptr<Item>>(); }
	};
}
#endif