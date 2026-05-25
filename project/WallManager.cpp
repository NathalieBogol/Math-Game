#include "WallManager.h"
#include "console.h"

void WallManager::reset(Screen& screen) {
    kWallTimer = 0;
    kOwnerIndex = -1;
    clearWall(screen);
}

void WallManager::tick(Screen& screen) {
    if (kWallTimer > 0) {
        kWallTimer--;
        if (kWallTimer == 0) {
            clearWall(screen);
            kOwnerIndex = -1;
        }
    }
}

void WallManager::applyKWall(int ownerIndex, int opponentIndex, Player players[], Items& items, Screen& screen) {
    clearWall(screen);
    kOwnerIndex = ownerIndex;
    kWallTimer = WALL_DURATION;
    createWall(opponentIndex, players, items, screen);
}

bool WallManager::shouldRespawn(int playerIndex, const Player& player, Screen& screen) const {
    if (kWallTimer == 0 || kOwnerIndex != playerIndex) {
        return false;
    }

    const Point& location = player.getLocation();
    return isWallCell(location.getX(), location.getY(), screen);
}
void WallManager::redraw(Player players[], Items& items, Screen& screen) {
    if (kWallTimer > 0) {
        createWall(kOwnerIndex == 0 ? 1 : 0, players, items, screen);
    }
}

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

bool WallManager::isWallCell(int x, int y, const Screen& screen) const {
    if (y < PLAY_MIN_Y || y > PLAY_MAX_Y) {
        return false;
    }
    return screen.isWall(Point(x, y, 0, 0, WALL_CHAR));
}


void WallManager::createWall(int opponentIndex, Player players[], Items& items, Screen& screen) {
    int centerX = players[opponentIndex].getLocation().getX();
    int centerY = players[opponentIndex].getLocation().getY();

    int left = centerX - WALL_OFFSET;
    int right = centerX + WALL_OFFSET;
    int top = centerY - WALL_OFFSET;
    int bottom = centerY + WALL_OFFSET;

    for (int y = top; y <= bottom; ++y) {
        if (y < PLAY_MIN_Y || y > PLAY_MAX_Y) {
            continue;
        }
        for (int x = left; x <= right; ++x) {
            int wrappedX = (x + Screen::MAX_X) % Screen::MAX_X;
            if (y == top || y == bottom || x == left || x == right) {
                screen.setCharAt(wrappedX, y, WALL_CHAR);
                items.removeAt(wrappedX, y);
                gotoxy(wrappedX, y);
                set_color(Color::LightPurple);
                std::cout << WALL_CHAR;
                reset_color();
            }
        }
    }
}
