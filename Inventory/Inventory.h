#ifndef INVENTORY
#define INVENTORY

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <sstream>

#include "Item.h"

namespace Inventory {

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

        InventoryData(int inventoryLimit)
                : inventoryLimit(inventoryLimit), currentInventoryCount(0), money(0) {
        }

        ~InventoryData() {
        }

        inline unsigned long getHeldMoney() {
            return money;
        }

        /**Returns the amount of money that was unable to be added to the inventory.
         */
        unsigned long addMoney(unsigned long money);

        /**Returns the amount of money that was not possible to remove form the inventory.
         */
        unsigned long removeMoney(unsigned long money);

        std::vector<std::shared_ptr<Item>> addAsPossible(std::vector<std::shared_ptr<Item>> items);
        std::shared_ptr<Item> addIfPossible(std::shared_ptr<Item> item);

        bool swapActiveArmor(int armorNumber);

        bool swapActiveWeapon(int weaponNumber);

        AttemptedUseStates useConsumable(int consumableNumber);

        const std::vector<std::shared_ptr<ConsumableItem>> getConsumables() const {
            return this->consumables;
        }

        const std::vector<std::shared_ptr<WeaponItem>> getOtherWeapons() const {
            return this->otherWeapons;
        }

        const std::vector<std::shared_ptr<ArmorItem>> getOtherArmors() const {
            return this->otherArmors;
        }

        bool hasActiveWeapon() const {
            return this->currentWeapon != nullptr;
        }

        const std::shared_ptr<WeaponItem> getCurrentWeapon() const {
            return this->currentWeapon;
        }

        std::shared_ptr<WeaponItem> removeCurrentWeapon();

        void unequipCurrentWeapon();

        std::shared_ptr<WeaponItem> removeWeapon(int number);

        bool hasActiveArmor() const {
            return this->currentArmor != nullptr;
        }

        const std::shared_ptr<ArmorItem> getCurrentArmor() const {
            return this->currentArmor;
        }

        std::shared_ptr<ArmorItem> removeCurrentArmor();

        void unequipCurrentArmor();

        std::shared_ptr<ArmorItem> removeArmor(int number);

        std::shared_ptr<ConsumableItem> removeConsumable(int number);

        friend std::ostream& operator<<(std::ostream& out, InventoryData& inv);

        std::string getCurrentWeaponString() {
            std::stringstream buff;
            if (this->hasActiveWeapon())
                buff << *this->getCurrentWeapon() << " (+" << this->getCurrentWeapon()->getPower() << ')';
            else
                buff << "no active weapon";
            return buff.str();
        }
        std::string getCurrentArmorString() {
            std::stringstream buff;
            if (this->hasActiveArmor())
                buff << *this->getCurrentArmor() << " (+" << this->getCurrentArmor()->getPower() << ')';
            else
                buff << "no active armor";
            return buff.str();
        }
        void printWeapons(std::ostream& out) const;

        void printArmors(std::ostream& out) const;

        void printConsumables(std::ostream& out) const;

        int getCurrentInventoryUsage() const {
            return this->currentInventoryCount;
        }

        int getInventoryCapacity() const {
            return this->inventoryLimit;
        }

        void addCapacity(int capacity) {
            this->inventoryLimit += capacity;
        }

        std::vector<std::shared_ptr<Item>> dropEverything();
    };
}
#endif