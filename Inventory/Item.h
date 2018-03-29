#ifndef MCGUFFINQUEST_ITEM_H
#define MCGUFFINQUEST_ITEM_H

#include <string>
#include <memory>
#include <functional>
#include <sstream>

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
                : type(type), displayName(displayName), baseWorth(baseWorth) {
        }

    public:
        virtual ~Item() {
        };

        inline ItemType getType() const {
            return type;
        }

        inline const std::string getName() const {
            return displayName;
        }

        friend std::ostream& operator<<(std::ostream& out, const Item& item) {
            return item.print(out);
        }
        std::string printToString() const {
            std::stringstream buff;
            buff << *this;
            return buff.str();
        }
        virtual std::ostream& print(std::ostream& out) const {
            return out << this->displayName;
        }

//        std::shared_ptr<Item> operator()() { return std::shared_ptr<Item>(this); }

        int getBaseWorth() const {
            return this->baseWorth;
        }
    };

    class MoneyItem : public Item {
        unsigned long worth;
    public:
        MoneyItem(unsigned long worth) : Item(ItemType::Money, MONEYNAME, 1), worth(worth) {
        }

        inline unsigned long getWorth() const {
            return worth;
        }

        inline bool isWorthless() {
            return worth == 0;
        }

        void reassignWorth(unsigned long worth) {
            this->worth = worth;
        }

        std::ostream& print(std::ostream& out) const {
            out << std::to_string(this->getWorth());
            out << std::string(" ");
            out << this->getName();
            return out;
        }
    };

    class WeaponItem : public Item {
        int power;
    public:
        WeaponItem(std::string name, int power, int worth)
                : Item(ItemType::Weapon, name, worth), power(power) {
        }

        int getPower() const {
            return this->power;
        }
        std::ostream& print(std::ostream& out) const {
            return Item::print(out) << " (+" << this->getPower() << ")   (" << std::to_string(this->getBaseWorth()) << MONEYNAME << ')';
        }
    };

    class ArmorItem : public Item {
        int power;
    public:
        ArmorItem(std::string name, int power, int worth)
                : Item(ItemType::Armor, name, worth), power(power) {
        }

        int getPower() const {
            return this->power;
        }
        std::ostream& print(std::ostream& out) const {
            return Item::print(out) << " (+" << this->getPower() << ")   (" << std::to_string(this->getBaseWorth()) << MONEYNAME << ')';
        }
    };

    class ConsumableItem : public Item {
        int count;
        const std::string desc;
    public:
        ConsumableItem(std::string name, std::string desc, int worth, int count = 1)
                : Item(ItemType::StackableConsumable, name, worth), desc(desc), count(count) {
        }

        /**Returns true if the two ConsumableItem(s) can (and have been) merge.
         * If the two are merged, the Consumable 'this' contains the merged items.
         */
        bool merge(std::shared_ptr<ConsumableItem> other);

        void setUses(int uses) {
            this->count = uses;
        }

        virtual bool mergeable(const std::shared_ptr<ConsumableItem> other) {
            return this->getName() == other->getName();
        }

        inline int getCount() const {
            return count;
        }

        virtual bool _use() = 0;

        bool attemptUse();

        std::ostream& print(std::ostream& out) const {
            out << this->getName();
            out << std::string(" (");
            out << std::to_string(this->count);
            out << std::string(")");
            return out;
        }

        virtual std::shared_ptr<ConsumableItem> clone() =0;

        const std::string getDescription() const {
            return this->desc;
        }
    };

    class ConsumableItem_Lambda : public ConsumableItem {
        std::function<bool()> useFunction;
    public:
        ConsumableItem_Lambda(std::function<bool()> useFunction, std::string name, std::string desc, int worth)
                : ConsumableItem(name, desc, worth), useFunction(useFunction) {
        }

        ConsumableItem_Lambda(std::function<bool()> useFunction, std::string name, std::string desc, int worth, int count)
                : ConsumableItem(name, desc, worth, count), useFunction(useFunction) {
        }

        virtual bool _use() {
            return this->useFunction();
        }

        virtual std::shared_ptr<ConsumableItem> clone();
    };
}

#endif //MCGUFFINQUEST_ITEM_H
