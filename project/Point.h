#pragma once

#include <iostream>
#include "Direction.h"
#include "console.h"

// Represents a drawable object with a board position and movement direction.
class Point {
	// Position, movement vector, displayed character, and display color.
	int x = 1, y = 1;
	int dir_x = 1, dir_y = 0;
	char ch = '*';
	Color color = Color::White;
public:
	// Creates a default white point at (1, 1), moving right.
	Point();
	// Creates a white point with the supplied position, movement vector, and character.
	Point(int x1, int y1, int dirx, int diry, char c);
	// Creates a point with the supplied position, movement vector, character, and color.
	Point(int x1, int y1, int dirx, int diry, char c, Color myColor);
	// Draws the point's stored character at its current position.
	void draw() {
		draw(ch);
	}
	// Draws a supplied character at the current position using the point's color.
	void draw(char c);
	// Moves one step in the current direction, wrapping around the playable board.
	void move();
	// Updates the movement vector for a direction.
	void setDirection(Direction dir);
	// Returns the Manhattan distance to another point.
	int distanceTo(const Point& p) const;
	// Returns the horizontal board coordinate.
	int getX() const {
		return x;
	}
	// Returns the vertical board coordinate.
	int getY() const {
		return y;
	}
	// Returns the point's stored display character.
	char getChar() const {
		return ch;
	}

	// Converts the stored movement vector to a Direction value.
	Direction getDirection() const {
		if (dir_y < 0) return Direction::UP;
		if (dir_x > 0) return Direction::RIGHT;
		if (dir_y > 0) return Direction::DOWN;
		if (dir_x < 0) return Direction::LEFT;
		return Direction::STAY;
	}
	// Updates the color used by draw.
   void setColor(Color newColor) {
		color = newColor;
	}
	// Returns true when two points have the same board coordinates.
	bool operator==(const Point& p) const
	{
		return this->x == p.x && this->y == p.y;
	}
};

