#include "Player.h"

int Player::Status() {
    //Win condition
    if (getCurrentHP() != 0)
        return 1;
    else
        return 0;
}