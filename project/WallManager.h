#pragma once
#include "Items.h"
#include "Player.h"
#include "Screen.h"

class WallManager {
public:
  void reset(Screen& screen);
    void tick(Screen& screen);
    void applyKWall(int ownerIndex, int opponentIndex, Player players[], Items& items, Screen& screen);
    void redraw(Player players[], Items& items, Screen& screen);
    bool isActive() const { return kWallTimer > 0; }
    int getOwnerIndex() const { return kOwnerIndex; }
    bool isWallCell(int x, int y, const Screen& screen) const;
    bool isInsideWallArea(int x, int y, int opponentX, int opponentY) const;

private:
    static constexpr int WALL_DURATION = 250;
    static constexpr int WALL_OFFSET = 5;
    static constexpr char WALL_CHAR = '#';
    static constexpr int RESPAWN_X = 10;
    static constexpr int RESPAWN_Y = 10;
    static constexpr int RESPAWN_X_ALT = 70;
    static constexpr int RESPAWN_Y_ALT = 10;
    static constexpr int PLAY_MIN_Y = 4;
    static constexpr int PLAY_MAX_Y = 19;

    int kWallTimer = 0;
    int kOwnerIndex = -1;
    void clearWall(Screen& screen);
    void createWall(int opponentIndex, Player players[], Items& items, Screen& screen);
};
