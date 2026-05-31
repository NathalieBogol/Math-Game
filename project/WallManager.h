#pragma once
#include "Items.h"
#include "Player.h"
#include "Screen.h"

class WallManager {
public:
    // Clears wall state and redraws the screen to remove any active wall
  void reset(Screen& screen);
    // Updates wall timers and clears wall when it expires
    void tick(Screen& screen);
    // Spawns a temporary wall around the opponent and updates items and screen
    void applyKWall(int ownerIndex, int opponentIndex, Player players[], Items& items, Screen& screen);
    // Redraws the wall and items onto the screen
    void redraw(Player players[], Items& items, Screen& screen);
    // Returns true if the wall is currently active
    bool isActive() const { return kWallTimer > 0; }
    // Returns the index of the player who owns the wall
    int getOwnerIndex() const { return kOwnerIndex; }
    // Returns true if the given cell is part of the wall
    bool isWallCell(int x, int y, const Screen& screen) const;
    // Returns true if a cell lies inside the active wall area
    bool isInsideWallArea(int x, int y, int opponentX, int opponentY) const;

private:
    static constexpr int WALL_DURATION = 25;
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
