#include "Graphics.h"

void Graphics::redrawMap() {
    int z = game.map.playerLocation().Z();

    std::cout << std::string(50, '\n');

    for (int y = 0; y <= MAP_Y_SIZE+1; y++) {
        std::cout << ']';
        for (int x = 0; x < MAP_X_SIZE; x++) {
            if (y == 0 || y == MAP_Y_SIZE+1)
                std::cout << "=";
            else
                std::cout << (game.map.getTile(x, y-1, z));
        }
        std::cout << "[  " << printInventory(y) << std::endl;
    }

    std::cout << std::endl;
}

void Graphics::show(VIEW view) {
    switch (view) {
    case VIEW_MAP:
        redrawMap();
        break;
    case VIEW_INVENTORY:
        // TODO: this
        break;
    }
}

std::string Graphics::printInventory(int line) {
    const char separatorChar = '#';
    unsigned long separatorLength = 76 - MAP_X_SIZE;
    const std::string separator = std::string(separatorLength, separatorChar);
    Inventory::InventoryData& inv = this->game.player.getInv();
    std::string temp;
    switch (line) {
    case 0:
        return separator;
    case 1:
        return "PLAYER INFO";
    case 2:
        return separator;
    case 3:
        return this->game.player.getName();
    case 4:
        return "" + std::to_string(inv.getHeldMoney()) + ' ' + Inventory::MONEYNAME;
    case 5:
        return "Health: " + std::to_string(this->game.player.getCurrentHP()) + " / " + std::to_string(this->game.player.getMaxHP());
    case 6:
        temp = "Attack: " + std::to_string(this->game.player.getCurrentBaseAttack());
        if (inv.hasActiveWeapon())
            temp += '+' + std::to_string(inv.getCurrentWeapon()->getPower());
        return temp;
    case 7:
        temp = "Defense: " + std::to_string(this->game.player.getCurrentBaseDefense());
        if (inv.hasActiveArmor())
            temp += '+' + std::to_string(inv.getCurrentArmor()->getPower());
        return temp;
    case 8:
        return "";
    case 9:
        return "Items:";
    case 10:
        return inv.getCurrentWeaponString();
    case 11:
        return inv.getCurrentArmorString();
    case 12:
        return "";
    case 13:

    default:
        int itemNumber = line - 10;
        // TODO: get player's items and display their name
        return itemNumber < 10 ? "<item " + std::to_string(itemNumber) + '>' : "";
    }
}
