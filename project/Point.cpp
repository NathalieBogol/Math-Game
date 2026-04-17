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
	x = (x + dir_x + Screen::MAX_X) % Screen::MAX_X;
	y = (y + dir_y + Screen::MAX_Y) % Screen::MAX_Y;
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
