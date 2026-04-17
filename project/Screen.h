#pragma once
#include "Point.h"
#include <string>

using std::cout, std::endl;

class Screen {
public:
	enum { MAX_X = 80, MAX_Y = 25 };
private:
	char screen[MAX_Y][MAX_X];
	char getCharAt(const Point& p) const {
		return screen[p.getY()][p.getX()];
	}
public:
	//constractor
	Screen();

	void draw() const;

	// is the player touch the wall
	bool isWall(const Point& p) const {
		return getCharAt(p) == '#';
	}
	//sets a single character at the specified (x,y) within the screen matrix
	void setCharAt(int x, int y, char ch);
	//inserts a string into the screen matrix, starting from the given coordinates
	void updateText(int start_x, int start_y, const std::string& text);

};

