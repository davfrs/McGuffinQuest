#include "Game.h"

bool torchLogic(Game& game) {
    COORDINATE3 location = game.map.playerLocation();
    Map map = game.map;
    bool activated = false;

    COORDINATE3 t = location.incrementX();
    if (t.isValid()) {
        if (map.isSquareRevealed(t)) {
            map.revealSquare_Coord3(t);
            activated = true;
        }
    }

    t = location.decrementX();
    if (t.isValid()) {
        if (map.isSquareRevealed(t)) {
            map.revealSquare_Coord3(t);
            activated = true;
        }
    }

    t = location.incrementY();
    if (t.isValid()) {
        if (map.isSquareRevealed(t)) {
            map.revealSquare_Coord3(t);
            activated = true;
        }
    }

    t = location.decrementY();
    if (t.isValid()) {
        if (map.isSquareRevealed(t)) {
            map.revealSquare_Coord3(t);
            activated = true;
        }
    }

    return activated;
}

Game::Game(std::string playerName, EntityStats playerStats)
        : player(playerName, playerStats), map() {
    consumables.emplace(TORCH_NAME, std::shared_ptr<Inventory::ConsumableItem>(new Inventory::ConsumableItem_Lambda([this]() { return torchLogic(*this); }, TORCH_NAME, TORCH_DESC, TORCH_WORTH)));
    consumables.emplace(POTION_NAME, std::shared_ptr<Inventory::ConsumableItem>(new Inventory::ConsumableItem_Lambda([this]() { return player.heal(POTION_HEAL); }, POTION_NAME, POTION_DESC, POTION_WORTH)));
    consumables.emplace(MAXPOTION_NAME, std::shared_ptr<Inventory::ConsumableItem>(new Inventory::ConsumableItem_Lambda([this]() { return player.heal(player.getMaxHP()); }, MAXPOTION_NAME, MAXPOTION_DESC, MAXPOTION_WORTH)));
}

std::shared_ptr<Inventory::ConsumableItem> Game::getConsumableItem(const std::string name) {
    auto potionLoc = this->consumables.find(POTION_NAME);
    if (potionLoc != this->consumables.end())
        return potionLoc->second->clone();
    return std::shared_ptr<Inventory::ConsumableItem>();
}