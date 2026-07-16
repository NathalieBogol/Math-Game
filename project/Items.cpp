#include "Items.h"
#include <string>
#include <cstdlib> 
#include <cmath>  

namespace {
    // Adds a character only when it is not already in the collection.
    void addUniqueChar(std::string& chars, char value) {
        if (chars.find(value) == std::string::npos) {
            chars += value;
        }
    }

    // Returns the next item needed to advance or repair the player's answer.
    char getAnswerProgressItem(const Player& player,
        const std::string& correctAnswer) {
        const std::string& answer = player.getCurrentAnswer();
        const bool isPrefix = answer.size() <= correctAnswer.size()
            && correctAnswer.compare(0, answer.size(), answer) == 0;

        if (isPrefix && answer.size() < correctAnswer.size()) {
            return correctAnswer[answer.size()];
        }
        if (!answer.empty()) {
            const std::string withoutLast = answer.substr(0, answer.size() - 1);
            const bool eraseRepairsPrefix = withoutLast.size() <= correctAnswer.size()
                && correctAnswer.compare(0, withoutLast.size(), withoutLast) == 0;
            return eraseRepairsPrefix ? 'e' : 'c';
        }
        return ' ';
    }
}

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
         const char specials[] = { 'e', 'c', '@', '#', '$', '*', '!', '^', 'K' };
         return specials[rand() % 9];
    }
}


// Prevent a full board from deadlocking by spawning needed items in the first
// place, including answer digits, correction items, score/life items, and
// opponent-answer deletion items, without replacing existing items.
std::string Items::getProgressItemChars(const Player& p1, const Player& p2,
    const std::string& correctAnswer) const {
    std::string progressItems;
    const char firstPlayerItem = getAnswerProgressItem(p1, correctAnswer);
    const char secondPlayerItem = getAnswerProgressItem(p2, correctAnswer);
    if (firstPlayerItem != ' ') addUniqueChar(progressItems, firstPlayerItem);
    if (secondPlayerItem != ' ') addUniqueChar(progressItems, secondPlayerItem);

    addUniqueChar(progressItems, '$');
    addUniqueChar(progressItems, '!');

    if (!p1.getCurrentAnswer().empty() || !p2.getCurrentAnswer().empty()) {
        addUniqueChar(progressItems, '@');
        addUniqueChar(progressItems, '#');
    }
    return progressItems;
}

bool Items::spawnSpecificItem(char itemChar, const Player& p1, const Player& p2,
    const Screen& screen) {
    if (itemCount >= MAX_ITEMS) return false;

    const int playableRows = 16;
    const int positionCount = Screen::MAX_X * playableRows;
    const int firstPosition = rand() % positionCount;
    for (int offset = 0; offset < positionCount; ++offset) {
        const int position = (firstPosition + offset) % positionCount;
        const int x = position % Screen::MAX_X;
        const int y = 4 + position / Screen::MAX_X;
        if (isValidSpawn(x, y, p1, p2, screen)) {
            items[itemCount] = Point(x, y, 0, 0, itemChar, getItemColor(itemChar));
            items[itemCount].draw();
            ++itemCount;
            return true;
        }
    }
    return false;
}

void Items::ensureProgressItem(const Player& p1, const Player& p2,
    const std::string& correctAnswer, const Screen& screen) {
    // Keep the original random gameplay while fewer than 10 items are present.
    if (itemCount < PROGRESS_GUARANTEE_THRESHOLD) return;

    const std::string progressItems = getProgressItemChars(p1, p2, correctAnswer);
    for (int i = 0; i < itemCount; ++i) {
        if (progressItems.find(items[i].getChar()) != std::string::npos) {
            return;
        }
    }
    if (itemCount >= MAX_ITEMS || progressItems.empty()) return;

    std::string directItems;
    const char firstPlayerItem = getAnswerProgressItem(p1, correctAnswer);
    const char secondPlayerItem = getAnswerProgressItem(p2, correctAnswer);
    if (firstPlayerItem != ' ') addUniqueChar(directItems, firstPlayerItem);
    if (secondPlayerItem != ' ') addUniqueChar(directItems, secondPlayerItem);

    const bool chooseDirectItem = !directItems.empty() && rand() % 4 != 0;
    const std::string& choices = chooseDirectItem ? directItems : progressItems;
    const char selectedItem = choices[rand() % choices.size()];
    spawnSpecificItem(selectedItem, p1, p2, screen);
}

Color Items::getItemColor(char itemChar) const {
    if (itemChar >= '0' && itemChar <= '9') return Color::LightYellow;
    switch (itemChar) {
    case '^': return Color::LightGreen;
    case 'e':
    case 'c':
    case '#':
    case '@': return Color::LightBlue;
    case '$': return Color::Green;
    case '*':
    case '!': return Color::LightRed;
    case 'K': return Color::LightPurple;
    default: return Color::White;
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
            Color itemColor = Color::White;
            if (itemChar >= '0' && itemChar <= '9') {
                itemColor = Color::LightYellow;
            } else {
                switch (itemChar) {
                case '^':
                    itemColor = Color::LightGreen;
                    break;
                case 'e':
                case 'c':
                case '#':
                case '@':
                    itemColor = Color::LightBlue;
                    break;
                case '$':
                    itemColor = Color::Green;
                    break;
                case '*':
                case '!':
                    itemColor = Color::LightRed;
                    break;
                case 'K':
                    itemColor = Color::LightPurple;
                    break;
                default:
                    itemColor = Color::White;
                    break;
                }
            }
            items[itemCount] = Point(x, y, 0, 0, itemChar, itemColor);
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


// read-only access to the item at a board position without changing game state.
char Items::getCharAt(int x, int y) const {
    for (int i = 0; i < itemCount; ++i) {
        if (items[i].getX() == x && items[i].getY() == y) {
            return items[i].getChar();
        }
    }
    return ' ';
}

// Removes all active items from the game state
void Items::clearAll() {
    itemCount = 0;
}

// Removes the item at the specified coordinates
void Items::removeAt(int x, int y) {
    for (int i = 0; i < itemCount; ++i) {
        if (items[i].getX() == x && items[i].getY() == y) {
            items[i] = items[itemCount - 1];
            itemCount--;
            return;
        }
    }
}
