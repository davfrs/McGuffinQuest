#include <iostream>
#include <memory>
#include <conio.h>

#include "Game.h"
#include "Entity.h"
#include "Graphics.h"


using namespace std;
using namespace Inventory;

/*void raw() {
    char input = 0;
    bool notPass = false;
    while (game.player.Status() == 1) {
        //player status is 0, so continue
        while (!notPass) {
            while (!(input = getch())); //wait for player input
                                        //arrow key input seems to be OS specific?
            notPass = playerAction(input);
        } //wait until the player performs a valid action
          //game.turnCount++;
          //Do enemies do anything? Or just exist?
    }
}*/
/*    COORDINATE3 incrementX()
{ return COORDINATE3(this->x + 1, this->y, this->z); }

COORDINATE3 decrementX()
{ return COORDINATE3(this->x - 1, this->y, this->z); }

COORDINATE3 incrementY()
{ return COORDINATE3(this->x, this->y + 1, this->z); }

COORDINATE3 decrementY()
{ return COORDINATE3(this->x, this->y - 1, this->z); }*/

/*bool playerAction(char action) {
    bool acted = false;
    if (action == '5') {
        acted = true;
    }
    else if (action == '6') {
        acted = game.player.moveNorth();
    }
    else if (action == '4') {
        acted = game.player.moveSouth();
    }
    else if (action == '8') {
        acted = game.player.moveWest();
    }
    else if (action == '2') {
        acted = game.player.moveEast();
    }
    else if (action == '>') {
        acted = game.player.moveDown();
    }
    else if (action == '<') {
        acted = game.player.moveUp();
    }
    return acted;
}*/
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

    while (!_kbhit());

    return game;
}
int main() {
    shared_ptr<Game> game = pregame();
    Graphics graphicDisplay(*game);

    do {
        graphicDisplay.show(VIEW::VIEW_MAP);

    } while (false);//((game->player.getCurrentHP > 0) && game->map.getTilePlayer() != GOALPOINT);
    cout << "In the middle of the room you just entered, you see a cardboard box with \"McGuffin\" scribbled on it in black marker." << endl;
    cout << "You open the box, but aren't sure what you're looking at. Regardless, this must be the prize you were sent to retrieve." << endl;
    cout << "You pick up the box, and walk back the way you came to the old man. He thanks you, but it hardly seems this quest was" << endl;
    cout<< "worth your time... (Press any key to close)" << endl;
    while(_kbhit());
}