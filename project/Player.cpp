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
void Player::draw() {
    location.draw();
}
void Player::erase() {
    location.draw(' ');
}

void Player::move() {
    erase();
    location.move();

    // If double speed is active, move again in the same direction
    if (speedCycles > 0) {
        location.move();
        speedCycles--;
    }

    draw();
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

void Player::addScore(int pointsToAdd) {
    score += pointsToAdd;
}

void Player::loseLife() {
    if (lives > 0) {
        lives--;
    }
}

void Player::addDigit(char digit) {
    currentAnswer += digit;
}

void Player::eraseLastDigit() {
    if (!currentAnswer.empty()) {
        currentAnswer.pop_back();
    }
}

void Player::clearAnswer() {
    currentAnswer.clear();
}

void Player::activateDoubleSpeed() {
    speedCycles = 40; 
}