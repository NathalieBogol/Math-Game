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
    player_B(Point(70, 10, 0, 0, 'B'), 'B', p_B_Keys),
    itemSpawnCounter(0),
    roundNumber(0) // Initialize round
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
    roundNumber = 0;
    exercise.generate(); // Generate first exercise
    items.clearAll();
    screen.draw();
}

// Game loop
void Game::manage_playing() {
    gotoxy(0, 1); 
    std::cout << exercise.getExerciseString();
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

    // Check if either player lost all lives
    if (player_A.getLives() <= 0) {
        announceWinner('B');
        current_status = GameStatus::MENU;
        return;
    }
    if (player_B.getLives() <= 0) {
        announceWinner('A');
        current_status = GameStatus::MENU;
        return;
    }

    // Spawn a new item every 20 frames
    itemSpawnCounter++;
    if (itemSpawnCounter >= 20) {
        items.spawnItem(player_A, player_B, screen);
        itemSpawnCounter = 0;
    }

    // Check for item collection for each player
    char collectedA = items.checkCollision(player_A.getLocation());
    if (collectedA >= '0' && collectedA <= '9') {
        player_A.addDigit(collectedA);
    } else if (collectedA != ' ') {
        processSpecialItem(player_A, player_B, collectedA);
    }

    char collectedB = items.checkCollision(player_B.getLocation());
    if (collectedB >= '0' && collectedB <= '9') {
        player_B.addDigit(collectedB);
    } else if (collectedB != ' ') {
        processSpecialItem(player_B, player_A, collectedB);
    }

    screen.draw();
    items.drawItems(); // Draw all items on the screen
    player_A.draw();
    player_B.draw();

    // Display player answers
    displayAnswers();

    // Show Player A info (bottom left)
    gotoxy(0, 24);
    std::cout << "A Score: " << player_A.getScore() << "  Lives: " << player_A.getLives() << "      ";

    // Show Player B info (bottom right)
    gotoxy(Screen::MAX_X - 20, 24);
    std::cout << "B Score: " << player_B.getScore() << "  Lives: " << player_B.getLives() << "      ";

    // Check if either player solved the exercise
    check_status();

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

void Game::processSpecialItem(Player& current, Player& opponent, char itemChar) {
    switch (itemChar) {
    case 'e': // erase last digit for this player
        current.eraseLastDigit();
        break;
    case 'c': // clear entire number for this player
        current.clearAnswer();
        break;
    case '@': // erase last digit for opponent
        opponent.eraseLastDigit();
        break;
    case '#': // clear entire number for opponent
        opponent.clearAnswer();
        break;
    case '$': // add points to this player
        current.addScore(POINTS_FOR_DOLLAR);
        break;
    case '*': // this player loses life, number cleared
        current.loseLife();
        current.clearAnswer();
        break;
    case '!': // opponent loses life, number cleared
        opponent.loseLife();
        opponent.clearAnswer();
        break;
    case '^': // current player's speed is doubled (for 40 cycles)
        current.activateDoubleSpeed();
        break;
    default:
        break;
    }
}

void Game::displayAnswers() {
    gotoxy(0, 2);
    std::cout << "A Answer: " << player_A.getCurrentAnswer() << "           ";

    gotoxy(Screen::MAX_X - 18, 2);
    std::cout << "B Answer: " << player_B.getCurrentAnswer() << "           ";
}

void Game::check_status() {
    // Check if player A solved the exercise
    if (exercise.isCorrect(player_A.getCurrentAnswer()) && player_A.getCurrentAnswer() != "") {
        player_A.addScore(POINTS_PER_SOLUTION);
        gotoxy(25, 12);
        std::cout << "Player A solved it!";
        sleep_ms(1500);

        if (player_A.getScore() >= WINNING_SCORE) {
            announceWinner('A');
            current_status = GameStatus::MENU;
        } else {
            nextRound();
        }
        return;
    }

    // Check if player B solved the exercise
    if (exercise.isCorrect(player_B.getCurrentAnswer()) && player_B.getCurrentAnswer() != "") {
        player_B.addScore(POINTS_PER_SOLUTION);
        gotoxy(25, 12);
        std::cout << "Player B solved it!";
        sleep_ms(1500);

        if (player_B.getScore() >= WINNING_SCORE) {
            announceWinner('B');
            current_status = GameStatus::MENU;
        } else {
            nextRound();
        }
        return;
    }
}

void Game::nextRound() {
    roundNumber++;
    if (roundNumber < TOTAL_ROUNDS) {
        // Clear answers and items, generate new exercise
        player_A.clearAnswer();
        player_B.clearAnswer();
        items.clearAll();
        exercise.generate();
        screen.draw();
    }
}

void Game::announceWinner(char winnerChar) {
    clrscr();
    gotoxy(25, 12);
    if (winnerChar == 'A') {
        std::cout << "*** PLAYER A WINS! ***";
    } else {
        std::cout << "*** PLAYER B WINS! ***";
    }
    sleep_ms(2000);
}