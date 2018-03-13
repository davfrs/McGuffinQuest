#pragma once

#ifndef INVENTORY
#define INVENTORY

#include <string>
#include <vector>
#include <functional>

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
	protected:
		Item(ItemType type, string displayName) :type(type), displayName(displayName) {}
	public:
		Item(const Item&) = delete;
		virtual ~Item() {};
		inline ItemType getType() const { return type; }
		inline const string getName() const { return displayName; }
		friend ostream& operator<<(ostream& out, const Item& item) { return item.print(out); }
		virtual ostream& print(ostream& out) const { return out << this->displayName; }
	};
	
	class MoneyItem : public Item {
		unsigned long worth;
	public:
		MoneyItem(unsigned long worth) : Item(ItemType::Money, MONEYNAME), worth(worth) {}
		inline unsigned long getWorth() const { return worth; }
		inline bool isWorthless() { return worth == 0; }
		void reassignWorth(unsigned long worth) { this->worth = worth; }
		virtual ostream& print(ostream& out) const { return out << this->getWorth() << ' ' << this->getName(); }
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
		virtual ostream& print(ostream& out) { return out << this->getName() << " ("+this->count+')'; }
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

		vector<reference_wrapper<ConsumableItem>> consumables;
		vector<reference_wrapper<WeaponItem>> otherWeapons;
		vector<reference_wrapper<ArmorItem>> otherArmors;
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
		vector<reference_wrapper<Item>> addAsPossible(vector<reference_wrapper<Item>> items);
		bool swapActiveArmor(int armorNumber);
		bool swapActiveWeapon(int weaponNumber);
		AttemptedUseStates useConsumable(int consumableNumber);
		
		const vector<reference_wrapper<ConsumableItem>> getConsumables()const { return this->consumables; }
		const vector<reference_wrapper<WeaponItem>> getOtherWeapons()const { return this->otherWeapons; }
		const vector<reference_wrapper<ArmorItem>> getOtherArmors()const { return this->otherArmors; }
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