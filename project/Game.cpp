#include "Game.h"
#include "console.h"
#include "Items.h"
#include <iostream>

//Each of the players keys
static const char p_A_Keys[5] = { 'w', 'd', 'x', 'a', 's' };
static const char p_B_Keys[5] = { 'i', 'l', 'm', 'j', 'k' };

//icor
Game::Game() :
    current_status(GameStatus::MENU),
    itemSpawnCounter(0),
    roundNumber(0),
    players{
        Player(Point(10, 10, 0, 0, 'A'), p_A_Keys),
        Player(Point(70, 10, 0, 0, 'B'), p_B_Keys)
    }
{
    set_colors_enabled(colorsEnabled);
}
//change name 
void Game::run() {
    size_t round = 0;
    while (current_status != GameStatus::EXIT) {
        switch (current_status) {
        case GameStatus::MENU:
            manage_menu();
            break;
        case GameStatus::INSTRUCTIONS:
            manage_instructions();
            break;
        case GameStatus::PLAYING:
            manage_playing(++round);
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
    int centerX = 80 / 2;
    int startY = 25 / 2 - 4;

    gotoxy(centerX - 8, startY);
    set_color(Color::LightYellow);
    std::cout << "=== MATH GAME ===";
    reset_color();

    gotoxy(centerX - 9, startY + 2);
    std::cout << "(1) Start a new game";

    gotoxy(centerX - 14, startY + 3);
    std::cout << "(8) Present instructions and keys";

    gotoxy(centerX - 11, startY + 4);
    std::cout << "(7) Colors Mode: " << (colorsEnabled ? "ON" : "OFF");

    gotoxy(centerX - 5, startY + 5);
    std::cout << "(9) EXIT";
}


void Game::manage_menu() {
    draw_menu();

    char choice = get_single_char();

    if (choice == '1') {
        reset_game();
        current_status = GameStatus::PLAYING;
    }
    else if (choice == '7') {
        colorsEnabled = !colorsEnabled;
        set_colors_enabled(colorsEnabled);
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
    set_color(Color::LightYellow);
    std::cout << "=== INSTRUCTIONS ===" << std::endl << std::endl;
    reset_color();
    std::cout << "Player 1 (A) Keys: W (Up), D (Right), X (Down), A (Left), S (Stay)" << std::endl;
    std::cout << "Player 2 (B) Keys: I (Up), L (Right), M (Down), J (Left), K (Stay)" << std::endl;
    std::cout << "Press ESC during the game to pause." << std::endl << std::endl;
    std::cout << "Press any key to return to the menu..." << std::endl;

   
    get_single_char();
    current_status = GameStatus::MENU;
}


void Game::reset_game() {
    players[0] = Player(Point(10, 10, 0, 0, 'A'), p_A_Keys);
    players[1] = Player(Point(70, 10, 0, 0, 'B'), p_B_Keys);
    roundNumber = 0;
    exercise.generate(); // Generate first exercise
    items.clearAll();
    screen.draw();
}

// Game loop
void Game::manage_playing(size_t round) {
    gotoxy(0, 1); 
    std::cout << exercise.getExerciseString();
    if (check_kbhit()) {
        char key = get_single_char();

        if (key == 27) { //ESCAPE
            current_status = GameStatus::PAUSED;
            return; 
        }
        players[0].keyPressed(key);
        players[1].keyPressed(key);
    }
	bool is_fast_round = (round % 2 == 0);
    players[0].move(is_fast_round);
    players[1].move(is_fast_round);

    // Check if either player lost all lives
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (players[i].getLives() <= 0) {
            int winner = (i == 0) ? 1 : 0;
            announceWinner('A' + winner);
            current_status = GameStatus::MENU;
            return;
        }
    }

    // Spawn a new item every 20 frames
    itemSpawnCounter++;
    if (itemSpawnCounter >= 20) {
        items.spawnItem(players[0], players[1], screen);
        itemSpawnCounter = 0;
    }

    // Check for item collection for each player
    for (int i = 0; i < NUM_PLAYERS; i++) {
        char collected = items.checkCollision(players[i].getLocation());
        int opponent = (i == 0) ? 1 : 0;

        if (collected >= '0' && collected <= '9') {
            players[i].addDigit(collected);
        } else if (collected != ' ') {
            processSpecialItem(players[i], players[opponent], collected);
        }
    }
   

    // Display player answers
    displayAnswers();

    // Show Player A info (bottom left)
    gotoxy(0, 24);
    reset_color();
    std::cout << "A Score: " << players[0].getScore() << "  ";
    if (players[0].getLives() == 1) {
        set_color(Color::LightRed);
    }
    std::cout << "Lives: " << players[0].getLives();
    reset_color();
    std::cout << "   Speed Cycles: " << players[0].getSpeedCycles() << "      ";

    // Show Player B info (bottom right)
    gotoxy(Screen::MAX_X - 38, 24);
    reset_color();
    std::cout << "B Score: " << players[1].getScore() << "  ";
    if (players[1].getLives() == 1) {
        set_color(Color::LightRed);
    }
    std::cout << "Lives: " << players[1].getLives();
    reset_color();
    std::cout << "   Speed Cycles: " << players[1].getSpeedCycles()<< "      ";
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
		gotoxy(10, 12);
		std::cout << "                                                                    " << std::endl;
		items.drawItems(); // Redraw items after pause
		players[0].draw();
		players[1].draw();
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
    std::cout << "A Answer: " << players[0].getCurrentAnswer() << "           ";

    gotoxy(Screen::MAX_X - 18, 2);
    std::cout << "B Answer: " << players[1].getCurrentAnswer() << "           ";
}

void Game::check_status() {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        bool solved = exercise.isCorrect(players[i].getCurrentAnswer()) && players[i].getCurrentAnswer() != "";
        if (solved) {
            players[i].addScore(POINTS_PER_SOLUTION);
            gotoxy(25, 12);
        set_color(Color::LightYellow);
        std::cout << "Player " << (char)('A' + i) << " solved it!";
        reset_color();
            sleep_ms(1500);
        }
        if (players[i].getScore() >= WINNING_SCORE) {
            announceWinner('A' + i);
            current_status = GameStatus::MENU;
            return;
        }
        if (solved) {
            nextRound();
            return;
        }
    }
}

void Game::nextRound() {
    roundNumber++;
    if (roundNumber < TOTAL_ROUNDS) {
        // Clear answers and items, generate new exercise
        for (int i = 0; i < NUM_PLAYERS; i++) {
            players[i].clearAnswer();
        }
        items.clearAll();
        exercise.generate();
        screen.draw();
    }
}

void Game::announceWinner(char winnerChar) {
    clrscr();
    gotoxy(25, 12);
    set_color(Color::LightYellow);
    if (winnerChar == 'A') {
        std::cout << "*** PLAYER A WINS! ***";
    } else {
        std::cout << "*** PLAYER B WINS! ***";
    }
    reset_color();
    sleep_ms(2000);
}