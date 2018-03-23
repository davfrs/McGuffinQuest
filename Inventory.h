#ifndef INVENTORY
#define INVENTORY

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace Inventory {

    const std::string MONEYNAME("gold");

    enum ItemType {
        Money,
        StackableConsumable,
        Weapon,
        Armor
    };

    class Item {
        const std::string displayName;
        const ItemType type;
        const int baseWorth;
    protected:
        Item(ItemType type, std::string displayName, int baseWorth)
                : type(type), displayName(displayName), baseWorth(baseWorth) {}

    public:
        virtual ~Item() {};

        inline ItemType getType() const { return type; }

        inline const std::string getName() const { return displayName; }

        friend std::ostream &operator<<(std::ostream &out, const Item &item) { return item.print(out); }

        virtual std::ostream &print(std::ostream &out) const { return out << this->displayName; }

//        std::shared_ptr<Item> operator()() { return std::shared_ptr<Item>(this); }

        int getBaseWorth() const { return this->baseWorth; }
    };

    class MoneyItem : public Item {
        unsigned long worth;
    public:
        MoneyItem(unsigned long worth) : Item(ItemType::Money, MONEYNAME, 1), worth(worth) {}

        inline unsigned long getWorth() const { return worth; }

        inline bool isWorthless() { return worth == 0; }

        void reassignWorth(unsigned long worth) { this->worth = worth; }

        virtual std::ostream &print(std::ostream &out) const {
            return out << this->getWorth() << ' ' << this->getName();
        }
    };

    class WeaponItem : public Item {
        int power;
    public:
        WeaponItem(std::string name, int power, int worth) : Item(ItemType::Weapon, name, worth), power(power) {}

        int getPower() { return this->power; }
    };

    class ArmorItem : public Item {
        int power;
    public:
        ArmorItem(std::string name, int power, int worth) : Item(ItemType::Armor, name, worth), power(power) {}

        int getPower() { return this->power; }
    };

    class ConsumableItem : public Item {
        int count;
        const std::string desc;
    public:
        ConsumableItem(std::string name, std::string desc, int worth, int count = 1)
                : Item(ItemType::StackableConsumable, name, worth), desc(desc), count(count) {}

        /**Returns true if the two ConsumableItem(s) can (and have been) merge.
         * If the two are merged, the Consumable 'this' contains the merged items.
         */
        bool merge(std::shared_ptr<ConsumableItem> other);

        void setUses(int uses) { this->count = uses; }

        virtual bool mergeable(const std::shared_ptr<ConsumableItem> other) {
            return this->getName() == other->getName();
        }

        inline int getCount() const { return count; }

        virtual bool _use() = 0;

        bool attemptUse();

        std::ostream &print(std::ostream &out) const { return out << this->getName() << " (" << this->count << ')'; }

        virtual std::shared_ptr<ConsumableItem> clone() =0;

        std::shared_ptr<ConsumableItem> operator()() { return std::shared_ptr<ConsumableItem>(this); }

        const std::string getDescription() const { return this->desc; }
    };

    class ConsumableItem_Lambda : public ConsumableItem {
        std::function<bool()> useFunction;
    public:
        ConsumableItem_Lambda(std::function<bool()> useFunction, std::string name, std::string desc, int worth)
                : ConsumableItem(name, desc, worth), useFunction(useFunction) {}

        ConsumableItem_Lambda(std::function<bool()> useFunction, std::string name, std::string desc, int worth,
                              int count)
                : ConsumableItem(name, desc, worth, count), useFunction(useFunction) {}

        virtual bool _use() { return this->useFunction(); }

        virtual std::shared_ptr<ConsumableItem> clone();
    };

    enum AttemptedUseStates {
        Used,
        Unusable,
        DoesNotExist
    };

    class InventoryData {
    private:
        unsigned long money;
        std::shared_ptr<WeaponItem> currentWeapon;
        std::shared_ptr<ArmorItem> currentArmor;

        std::vector<std::shared_ptr<ConsumableItem>> consumables;
        std::vector<std::shared_ptr<WeaponItem>> otherWeapons;
        std::vector<std::shared_ptr<ArmorItem>> otherArmors;
        int inventoryLimit;
        int currentInventoryCount;

    public:
        using ItemPointerContainer = std::vector<std::shared_ptr<Item>>;


        InventoryData(int inventoryLimit) : inventoryLimit(inventoryLimit), currentInventoryCount(0), money(0) {}

        ~InventoryData() {}

        inline unsigned long getHeldMoney() { return money; }

        /**Returns the amount of money that was unable to be added to the inventory.
         */
        unsigned long addMoney(unsigned long money);

        /**Returns the amount of money that was not possible to remove form the inventory.
         */
        unsigned long removeMoney(unsigned long money);

        std::vector<std::shared_ptr<Item>> addAsPossible(std::vector<std::shared_ptr<Item>> items);

        bool swapActiveArmor(int armorNumber);

        bool swapActiveWeapon(int weaponNumber);

        AttemptedUseStates useConsumable(int consumableNumber);

        const std::vector<std::shared_ptr<ConsumableItem>> getConsumables() const { return this->consumables; }

        const std::vector<std::shared_ptr<WeaponItem>> getOtherWeapons() const { return this->otherWeapons; }

        const std::vector<std::shared_ptr<ArmorItem>> getOtherArmors() const { return this->otherArmors; }

        bool hasActiveWeapon() const { return this->currentWeapon != nullptr; }

        const std::shared_ptr<WeaponItem> getCurrentWeapon() const { return this->currentWeapon; }

        std::shared_ptr<WeaponItem> removeCurrentWeapon();

        void unequipCurrentWeapon();

        std::shared_ptr<WeaponItem> removeWeapon(int number);

        bool hasActiveArmor() const { return this->currentArmor != nullptr; }

        const std::shared_ptr<ArmorItem> getCurrentArmor() const { return this->currentArmor; }

        std::shared_ptr<ArmorItem> removeCurrentArmor();

        void unequipCurrentArmor();

        std::shared_ptr<ArmorItem> removeArmor(int number);

        std::shared_ptr<ConsumableItem> removeConsumable(int number);

        friend std::ostream &operator<<(std::ostream &out, InventoryData &inv);

        void printWeapons(std::ostream &out) const;

        void printArmors(std::ostream &out) const;

        void printConsumables(std::ostream &out) const;

        int getCurrentInventoryUsage() const { return this->currentInventoryCount; }

        int getInventoryCapacity() const { return this->inventoryLimit; }

        void addCapacity(int capacity) { this->inventoryLimit += capacity; }

        std::vector<std::shared_ptr<Item>> dropEverything();

    };
}
#endif