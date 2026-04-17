#pragma once
#include "Point.h"
#include "Player.h"
#include "Screen.h"

class Item {
public:
    //max number of items on the screen at the same time
    static constexpr int MAX_ITEMS = 50;

private:
    // Static array to store the currently active items 
    Point items[MAX_ITEMS];

    // Counter keeping track of how many items are currently active
    int itemCount;

    // Checks if a generated location is valid according to game rules
    bool isValidSpawn(const Point& p, const Player& p1, const Player& p2, const Screen& screen) const;

    // Randomly selects an item character to spawn digits 0-9 or special chars
    char getRandomItemChar() const;

public:
    // Constructor initializes the item counter to zero
    Item() : itemCount(0) {}

    //Attempts to spawn a new item
    void spawnItem(const Player& p1, const Player& p2, const Screen& screen);

    //Iterates through the array and draws all active items
    void drawItems();

    //Checks if the player's location matches any item's location
    char checkCollision(const Point& playerLocation);

    //Clears all items from the screen
    void clearAll();
};