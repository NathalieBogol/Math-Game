#pragma once
#include "Point.h"
#include "Player.h"
#include "Screen.h"
#include <string>

class Items {
public:
    //max number of items on the screen at the same time
    static constexpr int MAX_ITEMS = 20;

private:
    static constexpr int PROGRESS_GUARANTEE_THRESHOLD = 10;

    // Static array to store the currently active items 
    Point items[MAX_ITEMS];

    // Counter keeping track of how many items are currently active
    int itemCount;

    // Checks if a generated location is valid according to game rules
    bool isValidSpawn(int x, int y, const Player& p1, const Player& p2, const Screen& screen) const;

    // Randomly selects an item character to spawn digits 0-9 or special chars
    char getRandomItemChar() const;

    std::string getProgressItemChars(const Player& p1, const Player& p2,
        const std::string& correctAnswer) const;
    bool spawnSpecificItem(char itemChar, const Player& p1, const Player& p2,
        const Screen& screen);
    Color getItemColor(char itemChar) const;

public:
    // Constructor initializes the item counter to zero
    Items() : itemCount(0) {}

    //Attempts to spawn a new item
    void spawnItem(const Player& p1, const Player& p2, const Screen& screen);

    // Adds a useful item if no current item can advance the game
    void ensureProgressItem(const Player& p1, const Player& p2,
        const std::string& correctAnswer, const Screen& screen);

    //Iterates through the array and draws all active items
    void drawItems();

    //Checks if the player's location matches any item's location
    char checkCollision(const Point& playerLocation);
    // Read-only board access for computer movement planning
    char getCharAt(int x, int y) const;
    //Remove item at specific coordinates
    void removeAt(int x, int y);

    //Clears all items from the screen
    void clearAll();
};
