#include "Graphics.h"
#include <iomanip>

void Graphics::clearScreen() {
    std::cout << std::string(50, '\n');
}

void Graphics::drawMap(bool merchant = false) {
    clearScreen();
    int z = game.map.playerLocation().Z();
    
    for (int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << ']';
        for (int x = 0; x < MAP_X_SIZE; x++) {
            if (y == 0 || y == MAP_Y_SIZE + 1)
                std::cout << "=";
            else
                std::cout << (game.map.getTile(x, y - 1, z));
        }
        std::cout << "[  " << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
    std::cout << std::endl;
    if (merchant)
        std::cout << "You discovered a wandering merchant! press e to trade" << std::endl;
    
}

void Graphics::drawCombat(Entity &enemy) {
    clearScreen();
    
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    
    for (int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << std::string(10, ' ');
        std::cout << std::setw(MAP_X_SIZE + 3 - 10) << std::left << printSideInfo(y, enemy, "ENEMY");
        std::cout.copyfmt(oldState);
        std::cout << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
    std::cout << "Battle Log:" << std::endl;
    for (int i = 0; i < combatTextAreaLineCount; i++) {
        std::cout << combatText[i] << std::endl;
    }
}

void Graphics::addCombatText(std::string text) {
    if (combatTextCount == combatTextAreaLineCount) {
        for (int i = 0; i < combatTextAreaLineCount - 1; i++) {
            combatText[i] = combatText[i + 1];
        }
        combatTextCount = combatTextAreaLineCount - 1;
    }
    combatText[combatTextCount] = text;
    combatTextCount++;
}

void Graphics::clearCombatText() {
    for (int i = 0; i < combatTextAreaLineCount; i++) {
        combatText[i] = "";
    }
    combatTextCount = 0;
}

void Graphics::printInventory_Base() {
    clearScreen();
    
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    std::string menu[]{"Inventory Menu", "1: Weapons", "2: Armors", "3: Consumables", "4: Back", ""};
    int menuItems = 6;
    for (int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << std::string(10, ' ');
        std::cout << std::setw(MAP_X_SIZE + 3 - 10) << std::left << menu[(y < menuItems ? y : menuItems - 1)];
        std::cout.copyfmt(oldState);
        std::cout << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
}

void Graphics::printInventory_Weapons(bool merchant) {
    clearScreen();
    
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    std::string menu[]{"Weapons Menu", "options: equip [#], drop [active/#],", ""};
    if (merchant)
        menu[1] += " buy [power], sell [#]";
    menu[1] += " back";
    int menuItems = 3;
    for (unsigned int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << std::string(10, ' ');
        std::cout << std::setw(MAP_X_SIZE + 3 - 10) << std::left
                  << (y < menuItems ? menu[y] : (y == menuItems ? game.player.getInv().getCurrentWeaponString() : (
                          y - menuItems <= game.player.getInv().getOtherWeapons().size() ?
                          (std::to_string(y - menuItems) + ": ") +
                          game.player.getInv().getOtherWeapons()[y - menuItems - 1]->printToString() : "")));
        std::cout.copyfmt(oldState);
        std::cout << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
}

void Graphics::printInventory_Armors(bool merchant) {
    clearScreen();
    
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    std::string menu[]{"Armors Menu", "options: equip [#], drop [active/#],", ""};
    if (merchant)
        menu[1] += " buy [power], sell [#]";
    menu[1] += " back";
    int menuItems = 3;
    for (int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << std::string(10, ' ');
        std::cout << std::setw(MAP_X_SIZE + 3 - 10) << std::left
                  << (y < menuItems ? menu[y] : (y == menuItems ? game.player.getInv().getCurrentArmorString() : (
                          y - menuItems <= game.player.getInv().getOtherArmors().size() ?
                          (std::to_string(y - menuItems) + ": ") +
                          game.player.getInv().getOtherArmors()[y - menuItems - 1]->printToString() : "")));
        std::cout.copyfmt(oldState);
        std::cout << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
}

void Graphics::printInventory_Consumables(bool merchant) {
    clearScreen();
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    std::string menu[]{"Consumables Menu", "options: use [#], drop [#],", ""};
    if (merchant)
        menu[1] += " buy [name] [count], sell [#]";
    menu[1] += " back, list";
    int menuItems = 3;
    for (int y = 0; y <= MAP_Y_SIZE + 1; y++) {
        std::cout << std::string(10, ' ');
        std::cout << std::setw(MAP_X_SIZE + 3 - 10) << std::left
                  << (y < menuItems ? menu[y] : (y - menuItems - 1 < game.player.getInv().getConsumables().size() ?
                                                 (std::to_string(y - menuItems) + ": ") +
                                                 game.player.getInv().getConsumables()[y - menuItems -
                                                                                       1]->printToString() : ""));
        std::cout.copyfmt(oldState);
        std::cout << printSideInfo(y, game.player, "PLAYER") << std::endl;
    }
}

void Graphics::printConsumablesList() {
    clearScreen();
    auto list = game.getConsumables();
    for (auto e : list) {
        std::cout << e->getName() << " (" << e->getBaseWorth() << " each) - " << e->getDescription() << std::endl;
    }
}

void Graphics::show(VIEW view, std::shared_ptr<Entity> entity, bool merchant = false) {
    switch (view) {
    case VIEW_MAP:
        drawMap(merchant);
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
    case VIEW_CONSUMABLES_LIST:
        printConsumablesList();
        break;
    case VIEW_COMBAT:
        drawCombat(*entity);
        break;
    }
}

std::string Graphics::printSideInfo(int line, Entity &entity, std::string typeName) {
    const char separatorChar = '#';
    unsigned long separatorLength = 76 - MAP_X_SIZE;
    const std::string separator = std::string(separatorLength, separatorChar);
    Inventory::InventoryData &inv = entity.getInv();
    std::string temp;
    switch (line) {
    case 0:
        return separator;
    case 1:
        return typeName + " INFO";
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
        return typeName == "PLAYER" ? std::to_string(inv.getOtherWeapons().size()) + " other weapons" : "";
    case 14:
        return typeName == "PLAYER" ? std::to_string(inv.getOtherArmors().size()) + " other armors" : "";
    case 15:
        return typeName == "PLAYER" ? std::to_string(inv.getConsumables().size()) + " consumables" : "";
    case 16:
        return typeName == "PLAYER" ? std::to_string(inv.getCurrentInventoryUsage()) + '/' +
                                      std::to_string(inv.getInventoryCapacity()) + " inactive inventory usage" : "";
    default:
        return "";
    }
}
