#include <iostream>
#include <memory>
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
void enemyEncounter(Game& game) {
    shared_ptr<Entity>enemy = game.generateRandomEnemy(game.map.playerLocation().Z());
    cout << "A " << enemy->getName() << " was in the room you entered!" << endl;
    while (game.player.getCurrentHP() > 0 && enemy->getCurrentHP() > 0) {
        char input = getCharInput();
        std::tuple<int, int, bool> attack;
        switch (input) {
        case' ':

            enemy->attackEntity(game.player);
            break;
        case'e':
            attack = game.player.attackEntity(*enemy);
            if (std::get<2>(attack)) {
                //enemy has died.
                game.player.resetActiveStats();
                auto couldNotFit = game.player.getInv().addAsPossible(enemy->getInv().dropEverything());
                //too bad for the items that wouldn't fit.
                game.map.__setTile(game.map.playerLocation(), FLOOR);
            } else {
                enemy->attackEntity(game.player);
            }
        }
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

            break;
        }
        if (newLocation.isValid() && oldLocation != newLocation) {
            char tile = game->map.updatePlayer(newLocation);
            switch (tile) {
            case MONSTER:
                graphicDisplay.show(VIEW::VIEW_MAP);
                enemyEncounter(*game);
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