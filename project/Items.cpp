#include "Items.h"
#include <cstdlib> 
#include <cmath>   

// Checks if a generated location is valid according to the exercise rules
bool Items::isValidSpawn(int x, int y, const Player& p1, const Player& p2, const Screen& screen) const {

    if (screen.isWall(Point(x, y, 0, 0, '*'))) {
        return false;
    }

    //Distance from players must be at least 3 
    int distP1 = std::abs(x - p1.getLocation().getX()) + std::abs(y - p1.getLocation().getY());
    int distP2 = std::abs(x - p2.getLocation().getX()) + std::abs(y - p2.getLocation().getY());

    if (distP1 < 3 || distP2 < 3) {
        return false;
    }

    //Cannot be adjacent to an existing item (above, below, left, right, or same spot)
    for (int i = 0; i < itemCount; ++i) {
        int dx = std::abs(x - items[i].getX());
        int dy = std::abs(y - items[i].getY());

        if ((dx == 0 && dy == 0) || (dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
            return false;
        }
    }

    return true;
}

//Generates a random character
char Items::getRandomItemChar() const {
    int roll = rand() % 100;

    if (roll < 75) {
        //75% to get a digit 0-9
        return '0' + (rand() % 10);
    }
    else {
        //25% to get a special char
        const char specials[] = { 'e', 'c', '@', '#', '$', '*', '!', '^' };
        return specials[rand() % 8];
    }
}

//Attempts to spawn an item up to 3 times
void Items::spawnItem(const Player& p1, const Player& p2, const Screen& screen) {
    if (itemCount >= MAX_ITEMS) {
        return; 
    }

    //Tries up to 3 times to find a valid spot
    for (int attempt = 0; attempt < 3; ++attempt) {
        int x = rand() % Screen::MAX_X;
        //Keep Y in the playable area rows 4 to 19 to avoid text areas
        int y = (rand() % 16) + 4;

        if (isValidSpawn(x, y, p1, p2, screen)) {
            char itemChar = getRandomItemChar();
            items[itemCount] = Point(x, y, 0, 0, itemChar);
            items[itemCount].draw();
            itemCount++;
            break; // Successfully spawned, exit the retry loop
        }
    }
}

// Draws all active items
void Items::drawItems() {
    for (int i = 0; i < itemCount; ++i) {
        items[i].draw();
    }
}

// Checks if a player hit an item
char Items::checkCollision(const Point& playerLocation) {
    for (int i = 0; i < itemCount; ++i) {
        if (items[i] == playerLocation) {
            char collected = items[i].getChar();
            items[i] = items[itemCount - 1];
            itemCount--;

            return collected;
        }
    }
    return ' '; 
}

void Items::clearAll() {
    itemCount = 0;
}