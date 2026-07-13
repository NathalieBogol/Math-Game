#pragma once

#include <string>
#include "Direction.h"

class Items;
class MathExercise;
class Player;
class Screen;

class ComputerController {
public:
    // Chooses a direction using read-only game information
    Direction chooseDirection(const Player& player, const MathExercise& exercise,
        const Items& items, const Screen& screen) const;

private:
    bool findShortestPath(const Player& player, const Items& items,
        const Screen& screen, const std::string& wantedItems,
        Direction& chosenDirection) const;
    std::string chooseWantedItems(const Player& player,
        const MathExercise& exercise) const;
    bool isSafeToCross(char item, const std::string& wantedItems) const;
};
