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
	Point();
	Point(int x1, int y1, int dirx, int diry, char c);
	Point(int x1, int y1, int dirx, int diry, char c, Color myColor);
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
   void setColor(Color newColor) {
		color = newColor;
	}
	bool operator==(const Point& p) const
	{
		return this->x == p.x && this->y == p.y;
	}
};

