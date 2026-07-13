#pragma once

#include "Point.h"
#include <string>

class Player
{
private:
	Point location; //current location and character
	char keys[5]; //up,down,right,left,stay

	int score;
	int lives;
	std::string currentAnswer;
	int speedCycles;

public:
 Player(const Point& startLocation, const char(&playersKeys)[5]);

	void draw();
	void erase();
	void move(bool isFastRound);
	void keyPressed(char key);
	void setDirection(Direction direction);

	//inline functions
	const Point& getLocation() const { return location; }
	Direction getDirection() const { return location.getDirection(); }
    void setLocation(const Point& newLocation);
	char getChar() const { return location.getChar(); }
	int getScore() const { return score; }
	int getLives() const { return lives; }
	const std::string& getCurrentAnswer() const { return currentAnswer; }

	void addScore(int pointsToAdd);
	void loseLife();
	void addDigit(char digit);
	void eraseLastDigit();  
	void clearAnswer();     
	void activateDoubleSpeed();
	int getSpeedCycles() const { return speedCycles; }

	bool isFast() const { return speedCycles > 0; }
};





