#include "ItemManager.h"

// Applies the effect of a collected item to the current or opponent player
void ItemManager::applyItem(Player& current, Player& opponent, char itemChar) {
    switch (itemChar) {
    case 'e':
        current.eraseLastDigit();
        break;
    case 'c':
        current.clearAnswer();
        break;
    case '@':
        opponent.eraseLastDigit();
        break;
    case '#':
        opponent.clearAnswer();
        break;
    case '$':
        current.addScore(5);
        break;
    case '*':
        current.loseLife();
        current.clearAnswer();
        break;
    case '!':
        opponent.loseLife();
        opponent.clearAnswer();
        break;
    case '^':
        current.activateDoubleSpeed();
        break;
    default:
        break;
    }
}
