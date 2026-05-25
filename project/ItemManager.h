#pragma once
#include "Player.h"

class ItemManager {
public:
    static void applyItem(Player& current, Player& opponent, char itemChar);
};
