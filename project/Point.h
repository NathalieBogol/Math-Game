#pragma once

#include <iostream>
#include "Direction.h"
#include "console.h"

class Point {
	int x = 1, y = 1;
	int dir_x = 1, dir_y = 0;
	char ch = '*';
	Color color = Color::White;
public:
	Point() {}
	Point(int x1, int y1, int dirx, int diry, char c)
		: Point(x1, y1, dirx, diry, c, Color::White) {
	}
	Point(int x1, int y1, int dirx, int diry, char c, Color myColor) {
		x = x1;
		y = y1;
		dir_x = dirx;
		dir_y = diry;
		ch = c;
		color = myColor;
	}
	void draw() {
		draw(ch);
	}
	void draw(char c);
	void move();
	void setDirection(Direction dir);
	int distanceTo(const Point& p) const;
	int getX() const {
		return x;
	}
	int getY() const {
		return y;
	}
	char getChar() const {
		return ch;
	}
	bool operator==(const Point& p) const
	{
		return this->x == p.x && this->y == p.y;
	}
};

