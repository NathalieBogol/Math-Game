#include "WallManager.h"
#include "console.h"

// Resets wall state and clears any wall characters from the screen
void WallManager::reset(Screen& screen) {
    kWallTimer = 0;
    kOwnerIndex = -1;
    clearWall(screen);
}

// Decrements the wall timer and clears the wall when it expires
void WallManager::tick(Screen& screen) {
    if (kWallTimer > 0) {
        kWallTimer--;
        if (kWallTimer == 0) {
            clearWall(screen);
            kOwnerIndex = -1;
        }
    }
}

// Activates a temporary wall around the opponent and removes overlapping items
void WallManager::applyKWall(int ownerIndex, int opponentIndex, Player players[], Items& items, Screen& screen) {
    clearWall(screen);
    kOwnerIndex = ownerIndex;
    kWallTimer = WALL_DURATION;
    createWall(opponentIndex, players, items, screen);
}

// Recreates the wall for the active owner after screen updates
void WallManager::redraw(Player players[], Items& items, Screen& screen) {
    if (kWallTimer > 0) {
        createWall(kOwnerIndex == 0 ? 1 : 0, players, items, screen);
    }
}

// Clears all wall characters from the playable area
void WallManager::clearWall(Screen& screen) {
    for (int y = PLAY_MIN_Y; y <= PLAY_MAX_Y; ++y) {
        for (int x = 0; x < Screen::MAX_X; ++x) {
            if (isWallCell(x, y, screen)) {
                screen.setCharAt(x, y, ' ');
                gotoxy(x, y);
                std::cout << ' ';
            }
        }
    }
}

// Returns true if the specified cell contains a wall character
bool WallManager::isWallCell(int x, int y, const Screen& screen) const {
    if (y < PLAY_MIN_Y || y > PLAY_MAX_Y) {
        return false;
    }
    return screen.isWall(Point(x, y, 0, 0, WALL_CHAR));
}


// Draws the wall around the opponent, wrapping within the play area
void WallManager::createWall(int opponentIndex, Player players[], Items& items, Screen& screen) {
    int centerX = players[opponentIndex].getLocation().getX();
    int centerY = players[opponentIndex].getLocation().getY();

    int left = centerX - WALL_OFFSET;
    int right = centerX + WALL_OFFSET;
    int top = centerY - WALL_OFFSET;
    int bottom = centerY + WALL_OFFSET;

    const int PLAY_HEIGHT = PLAY_MAX_Y - PLAY_MIN_Y + 1;

    for (int y = top; y <= bottom; ++y) {
        int wrappedY = PLAY_MIN_Y + ((y - PLAY_MIN_Y) % PLAY_HEIGHT + PLAY_HEIGHT) % PLAY_HEIGHT;
        for (int x = left; x <= right; ++x) {
            int wrappedX = (x + Screen::MAX_X) % Screen::MAX_X;
            items.removeAt(wrappedX, wrappedY);
            if (y == top || y == bottom || x == left || x == right) {
                screen.setCharAt(wrappedX, wrappedY, WALL_CHAR);
                gotoxy(wrappedX, wrappedY);
                set_color(Color::LightPurple);
                std::cout << WALL_CHAR;
                reset_color();
            } else {
                gotoxy(wrappedX, wrappedY);
                std::cout << ' ';
            }
        }
    }
}

// Returns true if a position lies within the wall bounds around the opponent.
bool WallManager::isInsideWallArea(int x, int y, int opponentX, int opponentY) const {
    int dx = x - opponentX;
    if (dx < 0) dx = -dx;
    // Horizontal wrapping
    if (dx > Screen::MAX_X / 2) dx = Screen::MAX_X - dx;

    int dy = y - opponentY;
    if (dy < 0) dy = -dy;
    // Vertical wrapping within play area
    const int PLAY_HEIGHT = PLAY_MAX_Y - PLAY_MIN_Y + 1;
    if (dy > PLAY_HEIGHT / 2) dy = PLAY_HEIGHT - dy;

    return dx <= WALL_OFFSET && dy <= WALL_OFFSET;
}
