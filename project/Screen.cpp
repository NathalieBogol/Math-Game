#include "Screen.h"
#include "console.h"
#include <iostream>

// constractor
Screen::Screen() {
    for (int y = 0; y < MAX_Y; ++y) {
        for (int x = 0; x < MAX_X; ++x) {
            screen[y][x] = ' ';
        }
    }
    for (int x = 0; x < MAX_X; ++x) {
        screen[3][x] = '#';
    }
    for (int x = 0; x < MAX_X; ++x) {
        screen[20][x] = '#';
    }
}

void Screen::setCharAt(int x, int y, char c) {
    if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {
        screen[y][x] = c;
    }
}

void Screen::updateText(int startX, int startY, const std::string& text) {
    if (startY >= 0 && startY < MAX_Y) {
        for (size_t i = 0; i < text.length(); ++i) {
            if (startX + i < MAX_X) {
                screen[startY][startX + i] = text[i];
            }
        }
    }
}

void Screen::draw() const {
    set_color(Color::White);

    for (int y = 0; y < MAX_Y; ++y) {
        gotoxy(0, y);
        for (int x = 0; x < MAX_X; ++x) {
            std::cout << screen[y][x];
        }
    }
    std::cout << std::flush; 
    reset_color();
}