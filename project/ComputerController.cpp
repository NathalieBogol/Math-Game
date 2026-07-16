#include "ComputerController.h"
#include "Items.h"
#include "MathExercise.h"
#include "Player.h"
#include "Screen.h"
#include <queue>

// This file controls the computer player's movement. It chooses useful items
// for the current math answer and searches the board for a safe shortest path.
namespace {
    // The playable rows exclude the screen's header and footer areas.
    constexpr int PLAY_MIN_Y = 4;
    constexpr int PLAY_MAX_Y = 19;
    constexpr int PLAY_HEIGHT = PLAY_MAX_Y - PLAY_MIN_Y + 1;

    // A queued board cell keeps the first move taken to reach that cell.
    struct SearchCell {
        int x;
        int y;
        Direction firstDirection;
    };
}

// Chooses the next needed item, then falls back to safe bonus items.
Direction ComputerController::chooseDirection(const Player& player,
    const MathExercise& exercise, const Items& items, const Screen& screen) const {
    Direction direction = Direction::STAY;
    const std::string wantedItems = chooseWantedItems(player, exercise);

    if (findShortestPath(player, items, screen, wantedItems, direction)) {
        return direction;
    }

    // While waiting for a needed digit or correction item, collect useful bonuses.
    if (findShortestPath(player, items, screen, "$^K!@#", direction)) {
        return direction;
    }
    return Direction::STAY;
}

// Determines whether the computer needs the next digit, an erase, or a correction.
std::string ComputerController::chooseWantedItems(const Player& player,
    const MathExercise& exercise) const {
    const std::string correctAnswer = exercise.getCorrectAnswerString();
    const std::string& currentAnswer = player.getCurrentAnswer();
    const bool isCorrectPrefix = currentAnswer.size() <= correctAnswer.size()
        && correctAnswer.compare(0, currentAnswer.size(), currentAnswer) == 0;

    if (isCorrectPrefix && currentAnswer.size() < correctAnswer.size()) {
        return std::string(1, correctAnswer[currentAnswer.size()]);
    }

    if (!currentAnswer.empty()) {
        const std::string withoutLastDigit = currentAnswer.substr(0, currentAnswer.size() - 1);
        const bool eraseFixesPrefix = withoutLastDigit.size() <= correctAnswer.size()
            && correctAnswer.compare(0, withoutLastDigit.size(), withoutLastDigit) == 0;
        return eraseFixesPrefix ? "e" : "c";
    }
    return "";
}

// Uses breadth-first search to find the first move toward a safe wanted item.
bool ComputerController::findShortestPath(const Player& player, const Items& items,
    const Screen& screen, const std::string& wantedItems,
    Direction& chosenDirection) const {
    if (wantedItems.empty()) {
        return false;
    }

    bool visited[Screen::MAX_Y][Screen::MAX_X] = {};
    std::queue<SearchCell> cells;
    const int startX = player.getLocation().getX();
    const int startY = player.getLocation().getY();
    visited[startY][startX] = true;

    const Direction directions[] = {
        Direction::UP, Direction::RIGHT, Direction::DOWN, Direction::LEFT
    };
    const int dx[] = { 0, 1, 0, -1 };
    const int dy[] = { -1, 0, 1, 0 };

    // Seed the search with each possible first move, including wraparound.
    for (int i = 0; i < 4; ++i) {
        int nextX = (startX + dx[i] + Screen::MAX_X) % Screen::MAX_X;
        int nextY = PLAY_MIN_Y +
            ((startY + dy[i] - PLAY_MIN_Y + PLAY_HEIGHT) % PLAY_HEIGHT);
        cells.push({ nextX, nextY, directions[i] });
    }

    while (!cells.empty()) {
        SearchCell current = cells.front();
        cells.pop();

        if (visited[current.y][current.x]) {
            continue;
        }
        visited[current.y][current.x] = true;

        Point position(current.x, current.y, 0, 0, ' ');
        const char item = items.getCharAt(current.x, current.y);
        if (screen.isWall(position) || !isSafeToCross(item, wantedItems)) {
            continue;
        }
        if (wantedItems.find(item) != std::string::npos) {
            chosenDirection = current.firstDirection;
            return true;
        }

        // Continue the search in the four cardinal directions.
        for (int i = 0; i < 4; ++i) {
            int nextX = (current.x + dx[i] + Screen::MAX_X) % Screen::MAX_X;
            int nextY = PLAY_MIN_Y +
                ((current.y + dy[i] - PLAY_MIN_Y + PLAY_HEIGHT) % PLAY_HEIGHT);
            if (!visited[nextY][nextX]) {
                cells.push({ nextX, nextY, current.firstDirection });
            }
        }
    }
    return false;
}

// Allows empty spaces, wanted items, and bonuses that do not alter the answer.
bool ComputerController::isSafeToCross(char item,
    const std::string& wantedItems) const {
    if (item == ' ' || wantedItems.find(item) != std::string::npos) {
        return true;
    }

    // These items cannot corrupt the computer's own collected answer.
    const std::string safeBonuses = "$^K!@#";
    return safeBonuses.find(item) != std::string::npos;
}
