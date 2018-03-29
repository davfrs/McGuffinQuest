#include "Graphics.h"
#include <iomanip>

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
        std::cout << "[  " << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
    std::cout << std::endl;
}

void Graphics::drawCombat(Entity& enemy) {
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    std::cout << std::string(50, '\n');

    for (int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << std::string(10, ' ');
        std::cout << std::setw(MAP_X_SIZE + 3 - 10) << std::left << printSideInfo(y, enemy, "ENEMY");
        std::cout.copyfmt(oldState);
        std::cout << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
    for (int i = 0; i < CombatText; i++) {
        std::cout << combatText[i] << std::endl;
    }
}
void Graphics::addCombatText(std::string text) {
    if (combatTextCount == CombatText) {
        for (int i = 0; i < CombatText - 1; i++) {
            combatText[i] = combatText[i + 1];
        }
        combatTextCount = CombatText - 1;
    }
    combatText[combatTextCount] = text;
    combatTextCount++;
}
void Graphics::clearCombatText() {
    for (int i = 0; i < CombatText; i++) {
        combatText[i] = "";
    }
    combatTextCount = 0;
}
void Graphics::printInventory_Base() {
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    std::cout << std::string(50, '\n');
    std::string menu[]{ "Inventory Menu","1: Weapons", "2: Armors", "3: Consumables", "4: Back", "" };
    int menuItems = 6;
    for (int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << std::string(10, ' ');
        std::cout << std::setw(MAP_X_SIZE + 3 - 10) << std::left << menu[(y < menuItems ? y : menuItems - 1)];
        std::cout.copyfmt(oldState);
        std::cout << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
}
void Graphics::printInventory_Weapons(bool merchant) {
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    std::cout << std::string(50, '\n');
    std::string menu[]{ "Weapons Menu", "options: equip, drop", "" };
    if (merchant)
        menu[1] += ", buy, sell";
    menu[1] += " [#]; back";
    int menuItems = 3;
    for (int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << std::string(10, ' ');
        std::cout << std::setw(MAP_X_SIZE + 3 - 10) << std::left << (y < menuItems ? menu[y] :(y==menuItems ? game.player.getInv().getCurrentWeaponString(): (y - menuItems < game.player.getInv().getOtherWeapons().size() ? (std::to_string(y - menuItems + 1) +": ")+ game.player.getInv().getOtherWeapons()[y - menuItems]->printToString() : "")));
        std::cout.copyfmt(oldState);
        std::cout << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
}
void Graphics::printInventory_Armors(bool merchant) {
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    std::cout << std::string(50, '\n');
    std::string menu[]{ "Armors Menu", "options: use, drop", "" };
    if (merchant)
        menu[1] += ", buy, sell";
    menu[1] += " [#]; back";
    int menuItems = 3;
    for (int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << std::string(10, ' ');
        std::cout << std::setw(MAP_X_SIZE + 3 - 10) << std::left << (y < menuItems ? menu[y] : (y == menuItems ? game.player.getInv().getCurrentArmorString() : (y - menuItems < game.player.getInv().getOtherArmors().size() ? (std::to_string(y - menuItems + 1) + ": ") + game.player.getInv().getOtherArmors()[y - menuItems]->printToString() : "")));
        std::cout.copyfmt(oldState);
        std::cout << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
}
void Graphics::printInventory_Consumables(bool merchant) {
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    std::cout << std::string(50, '\n');
    std::string menu[]{ "Consumables Menu", "options: use, drop", "" };
    if (merchant)
        menu[1] += ", buy, sell";
    menu[1] += " [#]; back";
    int menuItems = 3;
    for (int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << std::string(10, ' ');
        std::cout << std::setw(MAP_X_SIZE + 3 - 10) << std::left << (y < menuItems ? menu[y] : (y - menuItems < game.player.getInv().getConsumables().size() ? (std::to_string(y - menuItems + 1) + ": ") + game.player.getInv().getConsumables()[y - menuItems]->printToString() : ""));
        std::cout.copyfmt(oldState);
        std::cout << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
}
void Graphics::show(VIEW view, std::shared_ptr<Entity> entity, bool merchant) {
    switch (view) {
    case VIEW_MAP:
        redrawMap();
        break;
    case VIEW_INVENTORY:
        printInventory_Base();
        break;
    case VIEW_INVENTORY_WEAPONS:
        printInventory_Weapons(merchant);
        break;
    case VIEW_INVENTORY_ARMORS:
        printInventory_Armors(merchant);
        break;
    case VIEW_INVENTORY_CONSUMABLES:
        printInventory_Consumables(merchant);
        break;
    case VIEW_COMBAT:
        drawCombat(*entity);
        break;
    }
}

std::string Graphics::printSideInfo(int line, Entity& entity, std::string typeName) {
    const char separatorChar = '#';
    unsigned long separatorLength = 76 - MAP_X_SIZE;
    const std::string separator = std::string(separatorLength, separatorChar);
    Inventory::InventoryData& inv = entity.getInv();
    std::string temp;
    switch (line) {
    case 0:
        return separator;
    case 1:
        return typeName+ " INFO";
    case 2:
        return separator;
    case 3:
        return entity.getName();
    case 4:
        return "" + std::to_string(inv.getHeldMoney()) + ' ' + Inventory::MONEYNAME;
    case 5:
        return "Health: " + std::to_string(entity.getCurrentHP()) + " / " + std::to_string(entity.getMaxHP());
    case 6:
        temp = "Attack: " + std::to_string(entity.getCurrentBaseAttack());
        if (inv.hasActiveWeapon())
            temp += "(+" + std::to_string(inv.getCurrentWeapon()->getPower()) + ')';
        return temp;
    case 7:
        temp = "Defense: " + std::to_string(entity.getCurrentBaseDefense());
        if (inv.hasActiveArmor())
            temp += "(+" + std::to_string(inv.getCurrentArmor()->getPower()) + ')';
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
        return std::to_string(inv.getOtherWeapons().size()) + " other weapons";
    case 14:
        return std::to_string(inv.getOtherArmors().size()) + " other armors";
    case 15:
        return std::to_string(inv.getConsumables().size()) + " consumables";
    default:
        return "";
        /*int itemNumber = line - 10;
        // TODO: get player's items and display their name
        return itemNumber < 10 ? "<item " + std::to_string(itemNumber) + '>' : "";*/
    }
}
