#include "console.h"
#include "Point.h"
#include "Screen.h"

void Point::draw(char c) {
	gotoxy(x, y);
	set_color(color);
	std::cout << c << std::flush;
	reset_color();
}

void Point::move() {
    int nextX = (x + dir_x + Screen::MAX_X) % Screen::MAX_X;

    // Only tunnel in playable area (rows 4-19)
    int nextY = y + dir_y;
    if (dir_y != 0) {
        int playableRows = 16; // 19 - 4 + 1
        int relY = (nextY - 4 + playableRows) % playableRows;
        nextY = relY + 4;
    }

    // Check if destination is a wall (optional, if you want to block walls)
    if (nextY != 3 && nextY != 20) { // 3 and 20 are wall rows
        x = nextX;
        y = nextY;
    }
}

void Point::setDirection(Direction dir) {
	switch (dir) {
	case Direction::UP:
		dir_x = 0;
		dir_y = -1;
		break;
	case Direction::RIGHT:
		dir_x = 1;
		dir_y = 0;
		break;
	case Direction::DOWN:
		dir_x = 0;
		dir_y = 1;
		break;
	case Direction::LEFT:
		dir_x = -1;
		dir_y = 0;
		break;
	case Direction::STAY:
		dir_x = 0;
		dir_y = 0;
		break;
	}
}

int Point::distanceTo(const Point& p) const
{
	int diff_x = std::abs(this->x - p.x);
	int diff_y = std::abs(this->y - p.y);
	return diff_x + diff_y;
}
