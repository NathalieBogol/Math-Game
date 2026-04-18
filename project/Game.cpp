#include "Game.h"
#include "console.h"
#include "Item.h"
#include <iostream>

//Each of the players keys
static const char p_A_Keys[5] = { 'w', 'd', 'x', 'a', 's' };
static const char p_B_Keys[5] = { 'i', 'l', 'm', 'j', 'k' };

//icor
Game::Game() :
    current_status(GameStatus::MENU),
    // player A
    player_A(Point(10, 10, 0, 0, 'A'), 'A',p_A_Keys),
    // player B
    player_B(Point(70, 10, 0, 0, 'B'), 'B', p_B_Keys)
{ }

    void Game::run() {
    while (current_status != GameStatus::EXIT) {
        switch (current_status) {
        case GameStatus::MENU:
            manage_menu();
            break;
        case GameStatus::INSTRUCTIONS:
            manage_instructions();
            break;
        case GameStatus::PLAYING:
            manage_playing();
            break;
        case GameStatus::PAUSED:
            manage_pause();
            break;
        }
    }
}
//clears the screen and prints the menu
void Game::draw_menu() {
    clrscr();
    std::cout << "=== MATH GAME ===" << std::endl << std::endl;
    std::cout << "(1) Start a new game" << std::endl;
    std::cout << "(8) Present instructions and keys" << std::endl;
    std::cout << "(9) EXIT" << std::endl;
}


void Game::manage_menu() {
    draw_menu();

    char choice = get_single_char();

    if (choice == '1') {
        reset_game();
        current_status = GameStatus::PLAYING;
    }
    else if (choice == '8') {
        current_status = GameStatus::INSTRUCTIONS;
    }
    else if (choice == '9') {
        current_status = GameStatus::EXIT;
    }
}

//instractions screen
void Game::manage_instructions() {
    clrscr();
    std::cout << "=== INSTRUCTIONS ===" << std::endl << std::endl;
    std::cout << "Player 1 (A) Keys: W (Up), D (Right), X (Down), A (Left), S (Stay)" << std::endl;
    std::cout << "Player 2 (B) Keys: I (Up), L (Right), M (Down), J (Left), K (Stay)" << std::endl;
    std::cout << "Press ESC during the game to pause." << std::endl << std::endl;
    std::cout << "Press any key to return to the menu..." << std::endl;

   
    get_single_char();
    current_status = GameStatus::MENU;
}


void Game::reset_game() {
    player_A = Player(Point(10, 10, 0, 0, 'A'), 'A', p_A_Keys);
    player_B = Player(Point(70, 10, 0, 0, 'B'), 'B', p_B_Keys);
    items.clearAll();
    screen.draw();
}

// Game loop
void Game::manage_playing() {
  
    if (check_kbhit()) {
        char key = get_single_char();

        if (key == 27) { //ESCAPE
            current_status = GameStatus::PAUSED;
            return; 
        }
        player_A.keyPressed(key);
        player_B.keyPressed(key);
    }
    player_A.move();
    player_B.move();


    screen.draw();
    player_A.draw();
    player_B.draw();
    sleep_ms(50);
}

void Game::manage_pause() {
    gotoxy(10, 12);
    std::cout << "Game paused, press ESC again to continue or H to go to the main menu" << std::endl;

    char key = get_single_char();
    if (key == 27) { // ESC
        current_status = GameStatus::PLAYING;
    }
    else if (key == 'h' || key == 'H') {
        current_status = GameStatus::MENU;
    }
}