#include <iostream>
#include <memory>
#include <sstream>
//#include <conio.h>


#include "Game.h"
#include "Entity.h"
#include "Graphics.h"
#include "OSInputs.h"

using namespace std;
using namespace Inventory;

void clearKeyboard() {
    while (__kbhit())
        __getch();
}
void waitForKeyboard() {
    clearKeyboard();
    while (!__kbhit());
}
char getCharInput() {
    return __getch();
}

std::vector<string> tokenizeOnSpace(std::string input) {
    std::vector <string> tokens;
    std::stringstream check1(input);
    std::string intermediate;
    while (std::getline(check1, intermediate, ' ')) {
        tokens.push_back(intermediate);
    }
    return tokens;
}

shared_ptr<Game> pregame() {
    cout << "You find yourself standing in front of a pair of wooden doors partially blocking what is obviously the entrance to a" << endl;
    cout << "massive cave system. An old man is in front of the doors, as if gaurding it. While it is obvious that you could walk" << endl;
    cout << "around the doors into the cave, you approach the old man instead." << endl;
    cout << "\"What is your name, young one?\" the old man weakly asks." << endl;
    string name;
    getline(cin, name);
    cout << '"' << name << ",\" you reply." << endl;
    cout << "\"Well, welcome to the cave of... dagnabbit -- I forgot what this here cave was named! Anyways, it's filled with" << endl;
    cout << "monsters, and I need you to get in there and find my McGuffin! I don't remember what exactly it was or what it does," << endl;
    cout << "but you'll know you've found it once you see it.\"" << endl;
    cout << "You stare at the old man, obviously thinking his quest is lame, but gently shake your head and pass through the wood" << endl;
    cout << "doors regardless..." << endl;
    cin.clear();
    shared_ptr<Game> game(new Game(name, { 100, 10, 4 }));
    game->map.revealSquare(game->map.playerLocation());
    
    cout << "Press any key to continue...";
    waitForKeyboard();
    clearKeyboard();
    return game;
}
bool browseInventory(Game& game, Graphics& graphicDisplay, bool limitOneAction, bool merchant) {
    std::cout << std::string(50, '\n');
    VIEW currentView = VIEW_INVENTORY;
    int actions = 0;
    bool finished = false;
    while (!finished) {
        graphicDisplay.show(currentView, merchant);
        string inputstring;
        std::vector<string> tokenizedInput;
        std::shared_ptr<Inventory::Item> item;
        int number = 0;
        char input = 0;
        switch (currentView) {
        case VIEW_INVENTORY:
            input = getCharInput();
            switch (input) {
            case '1':
                currentView = VIEW_INVENTORY_WEAPONS;
                break;
            case '2':
                currentView = VIEW_INVENTORY_ARMORS;
                break;
            case '3':
                currentView = VIEW_INVENTORY_CONSUMABLES;
                break;
            case '4':
                finished = true;
                break;
            }
            break;
        case VIEW_INVENTORY_WEAPONS:
			std::getline(cin, inputstring);
            if (inputstring == "back") {
                currentView = VIEW_INVENTORY;
                break;
            }
			tokenizedInput = tokenizeOnSpace(inputstring);
            if (tokenizedInput.size() != 2)
                break;
            if (tokenizedInput[0] == "equip") {
                try {
                    number = std::stoi(tokenizedInput[1]);
                } catch (invalid_argument& e) {
                    break;
                }
                if (game.player.getInv().swapActiveWeapon(number-1))
                    actions++;
                break;
            }
            if (tokenizedInput[0] == "drop") {
                if (tokenizedInput[0] == "active" && game.player.getInv().hasActiveWeapon()) {
                    game.player.getInv().removeCurrentWeapon();
                    actions++;
                    break;
                }
                try {
                    number = std::stoi(tokenizedInput[1]);
                } catch (invalid_argument& e) {
                    break;
                }
                if (game.player.getInv().removeWeapon(number - 1))
                    actions++;
                break;
            }
            if (tokenizedInput[0] == "buy" && merchant){
                try {
                    number = std::stoi(tokenizedInput[1]);
                } catch (invalid_argument& e) {
                    break;
                }
                item = game.generateWeapon(number);
                
                break;
            }
            if (tokenizedInput[0] == "sell" && merchant) {
                if (tokenizedInput[0] == "active" && game.player.getInv().hasActiveWeapon()) {
                    game.player.getInv().addMoney(game.player.getInv().removeCurrentWeapon()->getBaseWorth());
                    actions++;
                    break;
                }
                try {
                    number = std::stoi(tokenizedInput[1]);
                } catch (invalid_argument& e) {
                    break;
                }
                if (item = game.player.getInv().removeWeapon(number - 1)) {
                    game.player.getInv().addMoney(item->getBaseWorth());
                    actions++;
                }
                break;
            }
            break;
        case VIEW_INVENTORY_ARMORS:
			std::getline(cin, inputstring);
            if (inputstring == "back") {
                currentView = VIEW_INVENTORY;
                break;
            }
            tokenizedInput = tokenizeOnSpace(inputstring);
            if (tokenizedInput.size() != 2)
                break;
            if (tokenizedInput[0] == "equip") {
                try {
                    number = std::stoi(tokenizedInput[1]);
                } catch (invalid_argument& e) {
                    break;
                }
                if (game.player.getInv().swapActiveArmor(number - 1))
                    actions++;
                break;
            }
            if (tokenizedInput[0] == "drop") {
                if (tokenizedInput[0] == "active" && game.player.getInv().hasActiveArmor()) {
                    game.player.getInv().removeCurrentArmor();
                    actions++;
                    break;
                }
                try {
                    number = std::stoi(tokenizedInput[1]);
                } catch (invalid_argument& e) {
                    break;
                }
                if (game.player.getInv().removeArmor(number - 1))
                    actions++;
                break;
            }
            if (tokenizedInput[0] == "buy" && merchant) {
                try {
                    number = std::stoi(tokenizedInput[1]);
                } catch (invalid_argument& e) {
                    break;
                }
                item = game.generateArmor(number);
                
                break;
            }
            if (tokenizedInput[0] == "sell" && merchant) {
                if (tokenizedInput[0] == "active" && game.player.getInv().hasActiveArmor()) {
                    game.player.getInv().addMoney(game.player.getInv().removeCurrentArmor()->getBaseWorth());
                    actions++;
                    break;
                }
                try {
                    number = std::stoi(tokenizedInput[1]);
                } catch (invalid_argument& e) {
                    break;
                }
                if (item = game.player.getInv().removeArmor(number - 1)) {
                    game.player.getInv().addMoney(item->getBaseWorth());
                    actions++;
                }
                break;
            }
            break;
        case VIEW_INVENTORY_CONSUMABLES:
			std::getline(cin, inputstring);
            if (inputstring == "back") {
                currentView = VIEW_INVENTORY;
                break;
            }
            if (inputstring == "list") {
                graphicDisplay.show(VIEW::VIEW_CONSUMABLES_LIST);
                cout << "(press any key to return to Consumables Menu)";
                getCharInput();
                break;
            }
            tokenizedInput = tokenizeOnSpace(inputstring);
            if (tokenizedInput.size() == 2) {
                if (tokenizedInput[0] == "use") {
                    break;
                }
                if (tokenizedInput[0] == "drop") {
                    break;
                }
            }
            if (tokenizedInput[0] == "buy" && merchant) {
                break;
            }
            if (tokenizedInput[0] == "sell" && merchant) {
                break;
            }
            break;
        }
        finished |= (limitOneAction && actions != 0);
    }
    return actions != 0;
}
void enemyEncounter(Game& game, Graphics& graphicDisplay) {
    shared_ptr<Entity>enemy = game.generateRandomEnemy(game.map.playerLocation().Z());
    int attack;
    int defense;
    bool kill;
    graphicDisplay.clearCombatText();
    graphicDisplay.addCombatText("A " + enemy->getName() +" was in the room you entered!");
    while (game.player.getCurrentHP() > 0 && enemy->getCurrentHP() > 0) {
        graphicDisplay.show(VIEW_COMBAT, enemy);
        char input = getCharInput();
        switch (input) {
        case' ':
            if (browseInventory(game, graphicDisplay, true, false)) {
                tie<int, int, bool>(attack, defense, kill) = enemy->attackEntity(game.player);
                graphicDisplay.addCombatText("The enemy " + enemy->getName() + " attacked with " + std::to_string(attack) + " strength, but you blocked with " + std::to_string(defense) + " defense.");
            }
            break;
        case'e':
            tie<int, int, bool>(attack, defense, kill) = game.player.attackEntity(*enemy);
            graphicDisplay.addCombatText("You attacked with " + std::to_string(attack) + " strength, but the enemy " + enemy->getName() + " blocked with " + std::to_string(defense) + " defense.");
            if (kill) {
                graphicDisplay.addCombatText("You killed the enemy " + enemy->getName() + ". (Press any key to return to the map)");
                graphicDisplay.show(VIEW_COMBAT, enemy);
                getCharInput();
                game.player.resetActiveStats();
                auto couldNotFit = game.player.getInv().addAsPossible(enemy->getInv().dropEverything());
                //too bad for the items that wouldn't fit.
                game.map.__setTile(game.map.playerLocation(), FLOOR);
            } else {
                tie<int, int, bool>(attack, defense, kill) = enemy->attackEntity(game.player);
                graphicDisplay.addCombatText("The enemy " + enemy->getName() + " attacked with " + std::to_string(attack) + " strength, but you blocked with " + std::to_string(defense) + " defense.");
            }
        }
    }
    if (game.player.getCurrentHP() > 0) {
        graphicDisplay.addCombatText("The enemy " + enemy->getName() + " has defeated you. (Pres any key to leave combat)");
        graphicDisplay.show(VIEW_COMBAT, enemy);
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    shared_ptr<Game> game = pregame();
    Graphics graphicDisplay(*game);
    do {
        graphicDisplay.show(VIEW::VIEW_MAP);
        COORDINATE3 oldLocation = game->map.playerLocation();
        COORDINATE3 newLocation = oldLocation;
        char input = getCharInput();
        char tile = 0;
        switch (input) {
        case'a':
            newLocation = oldLocation.decrementX();
            break;
        case'd':
            newLocation = oldLocation.incrementX();
            break;
        case'w':
            newLocation = oldLocation.decrementY();
            break;
        case's':
            newLocation = oldLocation.incrementY();
            break;
        case'e':
            tile = game->map.getTilePlayer();
            switch (tile) {
            case GO_DEEPER:
                game->map.warpToNextDeaperLevel();
                break;
            case GO_HIGHER:
                game->map.warpUpALevel();
                break;
            case MERCHANT:
                browseInventory(*game, graphicDisplay, false, true);
                break;
            case LOOT:

                break;
            case STATUE:

                break;
            case HEAL:

                break;
            }
            break;
        case' ':
            browseInventory(*game, graphicDisplay, false, game->map.getTilePlayer() == MERCHANT);
            break;
        }
        if (newLocation.isValid() && oldLocation != newLocation) {
            char tile = game->map.updatePlayer(newLocation);
            switch (tile) {
            case MONSTER:
                //graphicDisplay.show(VIEW::VIEW_MAP);
                enemyEncounter(*game, graphicDisplay);
                break;
            case TRAP:

                break;
            case WARP:

                break;
            }
        }
    } while ((game->player.getCurrentHP() > 0) && game->map.getTilePlayer() != GOALPOINT);
    graphicDisplay.show(VIEW::VIEW_MAP);
    if (game->map.getTilePlayer() == GOALPOINT) {
        cout << "In the middle of the room you just entered, you see a cardboard box with \"McGuffin\" scribbled on it in black marker." << endl;
        cout << "You open the box, but aren't sure what you're looking at. Regardless, this must be the prize you were sent to retrieve." << endl;
        cout << "You pick up the box, and walk back the way you came to the old man. He thanks you, but it hardly seems this quest was" << endl;
        cout << "worth your time... (Press any key to close)" << endl;
    } else {
        cout << "You find youself unable to move. You expected more pain as you lie on the ground, but the vast amount of adrenaline in" << endl;
        cout << "your body numbs it... for now. Eventually, enemies will start wandering into the room you are in, drawn by the smell of" << endl;
        cout << "blood, but you will already have died. \"Perhaps this quest wasn't below my skill level after all,\" you think to" << endl;
        cout << "yourself. (Press any key to close)" << endl;
    }
    waitForKeyboard();
}