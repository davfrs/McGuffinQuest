#ifndef PROJECT_GRAPHICS_H
#define PROJECT_GRAPHICS_H

#include <iostream>
#include <string>

#include "Entity.h"
#include "Game.h"
#include "Map/Map.h"
 
enum View {
    VIEW_MAP,
    VIEW_INVENTORY,
    VIEW_INVENTORY_WEAPONS,
    VIEW_INVENTORY_ARMORS,
    VIEW_INVENTORY_CONSUMABLES,
    VIEW_CONSUMABLES_LIST,
    VIEW_COMBAT
};

const int combatTextAreaLineCount = 5;
class Graphics {
private:
    Game& game;
    std::string combatText[combatTextAreaLineCount];
    int combatTextCount;

    void clearScreen();
    void drawMap(bool merchant);

    void drawCombat(Entity& enemy);

    std::string printSideInfo(int line, Entity& entity, std::string typeName);

    void printInventory_Base();
    void printInventory_Weapons(bool merchant);
    void printInventory_Armors(bool merchant);
    void printInventory_Consumables(bool merchant);
    void printConsumablesList();
    void show(View view, std::shared_ptr<Entity> entity, bool merchant);
public:
    Graphics(Game& game) : game(game), combatTextCount(0) {}
    void show(View view, std::shared_ptr<Entity> entity) {
        return show(view, entity, false);
    }
    void show(View view, bool merchant) {
        return show(view, 0, merchant);
    }
    void show(View view) {
        return show(view, 0, false);
    }
    void addCombatText(std::string text);
    void clearCombatText();
};

#endif //PROJECT_GRAPHICS_H
