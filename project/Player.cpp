#include "Player.h"
#include <cctype> //std::tolower

//Constructor
Player::Player(const Point& startLocation, const char(&playersKeys)[5])
    : location(startLocation), score(0), lives(3), speedCycles(0)
{
    for (int i = 0; i < 5; ++i) {
        keys[i] = std::tolower(playersKeys[i]);
    }
}
// Draws the player at the current location
void Player::draw() {
    location.draw();
}

// Updates the player location
void Player::setLocation(const Point& newLocation) {
    location = newLocation;
}
// Erases the player from the current location
void Player::erase() {
    location.draw(' ');
}

// Moves the player based on the current direction and speed state
void Player::move(bool isFastRound) {
	if (speedCycles == 0 && isFastRound) return;// Skip movement if it's a fast round and player isn't currently fast
    erase();
    location.move();
    if (speedCycles > 0) {
        location.setColor(Color::LightRed);
    } else {
        location.setColor(Color::White);
    }
    location.draw();
    if (speedCycles > 0) {
        speedCycles--;
	}
}

void Player::keyPressed(char key) {
    char lowerKey = std::tolower(key);

    if (lowerKey == keys[0]) {
        location.setDirection(Direction::UP);
    }
    else if (lowerKey == keys[1]) {
        location.setDirection(Direction::RIGHT);
    }
    else if (lowerKey == keys[2]) {
        location.setDirection(Direction::DOWN);
    }
    else if (lowerKey == keys[3]) {
        location.setDirection(Direction::LEFT);
    }
    else if (lowerKey == keys[4]) {
        location.setDirection(Direction::STAY);
    }
}

// Adds points to the player's score
void Player::addScore(int pointsToAdd) {
    score += pointsToAdd;
}

// Decrements a life 
void Player::loseLife() {
    if (lives > 0) {
        lives--;
    }
}

// Appends a digit to the current answer 
void Player::addDigit(char digit) {
    if (currentAnswer.size() < 6) {
        currentAnswer += digit;
    }
}

// Removes the last digit from the current answer 
void Player::eraseLastDigit() {
    if (!currentAnswer.empty()) {
        currentAnswer.pop_back();
    }
}

// Clears the current answer 
void Player::clearAnswer() {
    currentAnswer.clear();
}

// Activates double speed for the player
void Player::activateDoubleSpeed() {
    speedCycles = 40; 
}