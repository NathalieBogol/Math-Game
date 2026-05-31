#pragma once
#include "Player.h"

class ItemManager {
public:
    // Applies an item effect to the current and opponent players
    static void applyItem(Player& current, Player& opponent, char itemChar);
};
