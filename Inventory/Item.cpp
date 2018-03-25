#include "Item.h"

namespace Inventory {
    bool ConsumableItem::merge(std::shared_ptr<ConsumableItem> other) {
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

    std::shared_ptr<ConsumableItem> ConsumableItem_Lambda::clone() {
        return std::shared_ptr<ConsumableItem>(new ConsumableItem_Lambda(this->useFunction, this->getName(), this->getDescription(), this->getBaseWorth()));
    }
}