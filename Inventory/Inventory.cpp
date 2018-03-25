#include "Inventory.h"

namespace Inventory {
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
    std::shared_ptr<Item> InventoryData::addIfPossible(std::shared_ptr<Item> itemPtr) {
        ItemType type = itemPtr->getType();
        switch (type) {
        case Money: {
            auto moneyItem = std::static_pointer_cast<MoneyItem>(itemPtr);

            unsigned long worth = moneyItem->getWorth();
            unsigned long remaining = this->addMoney(worth);
            if (remaining == 0) {
            }
            else {
                //std::shared_ptr<Item> money);
                return std::shared_ptr<Item>(new MoneyItem(remaining));
            }
            break;
        }

        case StackableConsumable: {
            auto consumableItem = std::static_pointer_cast<ConsumableItem>(itemPtr);

            bool nadded = true;
            for (auto e : this->consumables) {
                if (e->merge(consumableItem)) {
                    nadded = false;
                    break;
                }
            }
            if (nadded) {
                if (this->inventoryLimit > this->currentInventoryCount) {
                    this->currentInventoryCount++;
                    this->consumables.push_back(consumableItem);
                }
                else {
                    return itemPtr;
                }
            }
            break;
        }

        case Weapon: {
            auto weapon = std::static_pointer_cast<WeaponItem>(itemPtr);
            if (this->currentWeapon == nullptr) {
                this->currentWeapon = weapon;
            }
            else {
                if (inventoryLimit > currentInventoryCount) {
                    this->currentInventoryCount++;
                    this->otherWeapons.push_back(weapon);
                }
                else {
                    return itemPtr;
                }
            }
            break;
        }

        case Armor: {
            auto armor = std::static_pointer_cast<ArmorItem>(itemPtr);

            if (this->currentArmor == nullptr) {
                this->currentArmor = armor;
            }
            else {
                if (inventoryLimit > currentInventoryCount) {
                    this->currentInventoryCount++;
                    this->otherArmors.push_back(armor);
                }
                else {
                    itemPtr;
                }
            }
            break;
        }
        }
        return std::shared_ptr<Item>();
    }

    std::vector<std::shared_ptr<Item>> InventoryData::addAsPossible(std::vector<std::shared_ptr<Item>> items) {
        ItemPointerContainer failed;
        for (auto itemPtr : items) {
            if (itemPtr == nullptr) {
                continue;
            }
            auto result = this->addIfPossible(itemPtr);
            if (result != nullptr)
                failed.push_back(result);
        }

        return failed;
    }

    bool InventoryData::swapActiveArmor(int armorNumber) {
        if (this->otherArmors.size() <= (unsigned) armorNumber || armorNumber < 0)
            return false;
        if (this->currentArmor == nullptr) {
            this->currentArmor = this->otherArmors[armorNumber];
            this->otherArmors.erase(this->otherArmors.begin() + armorNumber);
            this->currentInventoryCount--;
        }
        else {
            this->otherArmors[armorNumber].swap(this->currentArmor);
        }
        return true;
    }

    bool InventoryData::swapActiveWeapon(int weaponNumber) {
        if (this->otherWeapons.size() <= (unsigned) weaponNumber || weaponNumber < 0)
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
        if (this->consumables.size() <= (unsigned) consumableNumber || consumableNumber < 0)
            return AttemptedUseStates::DoesNotExist;
        std::shared_ptr<ConsumableItem> consumable = this->consumables[consumableNumber];
        if (consumable->attemptUse()) {
            if (consumable->getCount() <= 0) {
                this->consumables.erase(this->consumables.begin() + consumableNumber);
                this->currentInventoryCount--;
            }
            return AttemptedUseStates::Used;
        }
        return AttemptedUseStates::Unusable;
    }

    std::shared_ptr<WeaponItem> InventoryData::removeCurrentWeapon() {
        std::shared_ptr<WeaponItem> ret = this->currentWeapon;
        this->currentWeapon.reset();
        return ret;
    }

    std::shared_ptr<ArmorItem> InventoryData::removeCurrentArmor() {
        std::shared_ptr<ArmorItem> ret = this->currentArmor;
        this->currentWeapon.reset();
        return ret;
    }

    std::shared_ptr<WeaponItem> InventoryData::removeWeapon(int weaponNumber) {
        if (this->otherWeapons.size() <= (unsigned) weaponNumber || weaponNumber < 0)
            return std::shared_ptr<WeaponItem>();
        std::shared_ptr<WeaponItem> ret = this->otherWeapons[weaponNumber];
        this->otherWeapons.erase(this->otherWeapons.begin() + weaponNumber);
        this->currentInventoryCount--;
        return ret;
    }

    std::shared_ptr<ArmorItem> InventoryData::removeArmor(int armorNumber) {
        if (this->otherArmors.size() <= (unsigned) armorNumber || armorNumber < 0)
            return std::shared_ptr<ArmorItem>();
        std::shared_ptr<ArmorItem> ret = this->otherArmors[armorNumber];
        this->otherArmors.erase(this->otherArmors.begin() + armorNumber);
        this->currentInventoryCount--;
        return ret;
    }

    std::shared_ptr<ConsumableItem> InventoryData::removeConsumable(int consumableNumber) {
        if (this->consumables.size() <= (unsigned) consumableNumber || consumableNumber < 0)
            return std::shared_ptr<ConsumableItem>();
        std::shared_ptr<ConsumableItem> ret = this->consumables[consumableNumber];
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

    std::ostream& operator<<(std::ostream& out, InventoryData& inv) {
        out << inv.money << ' ' << MONEYNAME << std::endl;

        if (inv.hasActiveWeapon()) {
            out << *inv.getCurrentWeapon();
        }
        else {
            out << "no weapon";
        }
        out << std::endl;

        if (inv.hasActiveArmor()) {
            out << *inv.getCurrentArmor();
        }
        else {
            out << "no armor";
        }
        out << std::endl;

        out << "inventory space: " << inv.currentInventoryCount << '/' << inv.inventoryLimit
            << std::endl;

        return out;
    }

    void InventoryData::printWeapons(std::ostream& out) const {
        out << "Active Weapon:" << std::endl;
        if (this->hasActiveWeapon()) {
            out << *this->getCurrentWeapon() << std::endl;
        }
        else {
            out << "no active weapon" << std::endl;
        }
        out << "Other Weapons:" << std::endl;
        for (std::shared_ptr<WeaponItem> each : this->getOtherWeapons()) {
            out << *each << std::endl;
        }
    }

    void InventoryData::printArmors(std::ostream& out) const {
        out << "Active Armor:" << std::endl;
        if (this->hasActiveArmor()) {
            out << *this->getCurrentArmor() << std::endl;
        }
        else {
            out << "no active armor" << std::endl;
        }
        out << "Other Armors:" << std::endl;
        for (std::shared_ptr<ArmorItem> each : this->getOtherArmors()) {
            out << *each << std::endl;
        }
    }

    void InventoryData::printConsumables(std::ostream& out) const {
        for (std::shared_ptr<ConsumableItem> each : this->getConsumables()) {
            out << *each << std::endl;
        }
    }

    std::vector<std::shared_ptr<Item>> InventoryData::dropEverything() {
        std::vector<std::shared_ptr<Item>> ret;
        if (this->money != 0) {
            ret.push_back(std::make_shared<Item>(MoneyItem(this->money)));
            this->money = 0;
        }
        if (this->currentArmor != nullptr) {
            std::shared_ptr<Item> t = this->currentArmor;
            ret.push_back(t);
            this->currentArmor.reset();
        }
        if (this->currentWeapon != nullptr) {
            std::shared_ptr<Item> t = this->currentWeapon;
            ret.push_back(t);
            this->currentWeapon.reset();
        }
        for (std::shared_ptr<Item> e : this->otherWeapons) {
            std::shared_ptr<Item> e2 = e;
            ret.push_back(e2);
        }
        for (std::shared_ptr<Item> e : this->otherArmors) {
            std::shared_ptr<Item> e2 = e;
            ret.push_back(e2);
        }
        for (std::shared_ptr<Item> e : this->consumables) {
            std::shared_ptr<Item> e2 = e;
            ret.push_back(e2);
        }

        return ret;
    }
}