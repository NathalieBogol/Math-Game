#include "Game.h"
#include <string>
#include "console.h"
#include "Items.h"
#include "ItemManager.h"
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
    int centerX = MENU_CENTER_X;
    int startY = MENU_START_Y;

    gotoxy(centerX - MENU_TITLE_OFFSET, startY);
    set_color(Color::LightYellow);
    std::cout << "=== MATH GAME ===";
    reset_color();

    gotoxy(centerX - MENU_OPTION1_OFFSET, startY + 2);
    std::cout << "(" << (char)MENU_START << ") Start a new game";

    gotoxy(centerX - MENU_OPTION2_OFFSET, startY + 3);
    std::cout << "(" << (char)MENU_SELECT_LEVEL << ") Select level [" << levelToString(currentLevel) << "]";

    gotoxy(centerX - MENU_OPTION2_OFFSET, startY + 4);
    std::cout << "(" << (char)MENU_SELECT_OPERATION << ") Select operation [" << operationToString(currentOperation) << "]";

    gotoxy(centerX - MENU_OPTION2_OFFSET, startY + 5);
    std::cout << "(" << (char)MENU_INSTRUCTIONS << ") Present instructions and keys";

    gotoxy(centerX - MENU_OPTION3_OFFSET, startY + 6);
    std::cout << "(" << (char)MENU_TOGGLE_COLORS << ") Colors Mode: " << (colorsEnabled ? "ON" : "OFF");

    gotoxy(centerX - MENU_OPTION4_OFFSET, startY + 7);
    std::cout << "(" << (char)MENU_EXIT << ") EXIT";
}

// Returns a display string for the current difficulty level
const char* Game::levelToString(Level level) {
    switch (level) {
    case Level::EASY:
        return "Easy";
    case Level::MEDIUM:
        return "Medium";
    case Level::HARD:
        return "Hard";
    default:
        return "Easy";
    }
}

// Returns a display string for the current math operation
const char* Game::operationToString(Operation operation) {
    switch (operation) {
    case Operation::ADD:
        return "+";
    case Operation::SUBTRACT:
        return "-";
    case Operation::MULTIPLY:
        return "*";
    case Operation::DIVIDE:
        return "/";
    case Operation::EQUATION:
        return "Eq";
    default:
        return "+";
    }
}

void Game::selectOperation() {
    switch (currentOperation) {
    case Operation::ADD:
        currentOperation = Operation::SUBTRACT;
        break;
    case Operation::SUBTRACT:
        currentOperation = Operation::MULTIPLY;
        break;
    case Operation::MULTIPLY:
        currentOperation = Operation::DIVIDE;
        break;
    case Operation::DIVIDE:
        currentOperation = Operation::EQUATION;
        break;
    case Operation::EQUATION:
        currentOperation = Operation::ADD;
        break;
    default:
        currentOperation = Operation::ADD;
        break;
    }
}

void Game::selectLevel() {
    switch (currentLevel) {
    case Level::EASY:
        currentLevel = Level::MEDIUM;
        break;
    case Level::MEDIUM:
        currentLevel = Level::HARD;
        break;
    case Level::HARD:
        currentLevel = Level::EASY;
        break;
    default:
        currentLevel = Level::EASY;
        break;
    }
}


void Game::manage_menu() {
    draw_menu();

    char choice = get_single_char();

    switch (static_cast<MenuChoice>(choice)) {
    case MENU_START:
        reset_game();
        current_status = GameStatus::PLAYING;
        break;
    case MENU_SELECT_LEVEL:
        selectLevel();
        break;
    case MENU_SELECT_OPERATION:
        selectOperation();
        break;
    case MENU_TOGGLE_COLORS:
        colorsEnabled = !colorsEnabled;
        set_colors_enabled(colorsEnabled);
        break;
    case MENU_INSTRUCTIONS:
        current_status = GameStatus::INSTRUCTIONS;
        break;
    case MENU_EXIT:
        current_status = GameStatus::EXIT;
        break;
    default:
        break;
    }
}

//instractions screen
void Game::manage_instructions() {
    clrscr();
    set_color(Color::LightYellow);
    std::cout << "=== INSTRUCTIONS ===" << std::endl << std::endl;
    reset_color();
    std::cout << "GAME OVERVIEW:" << std::endl;
    std::cout << "Number of Rounds: 3" << std::endl;
    std::cout << "Winning Score: 30 points" << std::endl << std::endl;

    std::cout << "GAME SETTINGS:" << std::endl;
    std::cout << "Use the menu to choose the level: Easy, Medium, or Hard" << std::endl;
    std::cout << "Use the menu to choose the operation: +, -, *, /, or Eq" << std::endl << std::endl;

    std::cout << "PLAYER KEYS:" << std::endl;
    std::cout << "Player 1 (A) Keys: W (Up), D (Right), X (Down), A (Left), S (Stay)" << std::endl;
    std::cout << "Player 2 (B) Keys: I (Up), L (Right), M (Down), J (Left), K (Stay)" << std::endl << std::endl;

    std::cout << "SPECIAL CHARACTERS:" << std::endl;
    std::cout << "e - Erase your last digit" << std::endl;
    std::cout << "c - Clear your entire number" << std::endl;
    std::cout << "@ - Erase opponent's last digit" << std::endl;
    std::cout << "# - Clear opponent's entire number" << std::endl;
    std::cout << "$ - Add bonus points to your score" << std::endl;
    std::cout << "* - You lose a life and your number is cleared" << std::endl;
    std::cout << "! - Opponent loses a life and their number is cleared" << std::endl;
    std::cout << "^ - Double your movement speed for 40 cycles" << std::endl;
    std::cout << "K - Surrounds the opponent with a wall for 25 cycles" << std::endl;
    std::cout << "If the player that selected the K appears there, he will lose a life and start in another position" << std::endl << std::endl;
    std::cout << "Press ESC during the game to pause." << std::endl << std::endl;
    std::cout << "Press any key to return to the menu..." << std::endl;


    get_single_char();
    current_status = GameStatus::MENU;
}


void Game::reset_game() {
    players[0] = Player(Point(10, 10, 0, 0, 'A'), p_A_Keys);
    players[1] = Player(Point(70, 10, 0, 0, 'B'), p_B_Keys);
    roundNumber = 0;
    wallManager.reset(screen);
    exercise.generate(currentLevel, currentOperation); // Generate first exercise
    items.clearAll();
    screen.draw();
}

// Game loop
void Game::manage_playing(size_t round) {
    gotoxy(0, 1); 
    std::cout << exercise.getExerciseString() << "                ";

    wallManager.tick(screen);
    if (check_kbhit()) {
        char key = get_single_char();

        if (key == ESC) { //ESCAPE
            current_status = GameStatus::PAUSED;
            return; 
        }
        players[0].keyPressed(key);
        players[1].keyPressed(key);
    }
    // Solution to players not moving when keyboard on Hebrew. Used AI to find the solution.
#ifdef PLATFORM_WINDOWS
    if (GetAsyncKeyState('W') & 0x8000) players[0].keyPressed('w');
    if (GetAsyncKeyState('D') & 0x8000) players[0].keyPressed('d');
    if (GetAsyncKeyState('X') & 0x8000) players[0].keyPressed('x');
    if (GetAsyncKeyState('A') & 0x8000) players[0].keyPressed('a');
    if (GetAsyncKeyState('S') & 0x8000) players[0].keyPressed('s');

    if (GetAsyncKeyState('I') & 0x8000) players[1].keyPressed('i');
    if (GetAsyncKeyState('L') & 0x8000) players[1].keyPressed('l');
    if (GetAsyncKeyState('M') & 0x8000) players[1].keyPressed('m');
    if (GetAsyncKeyState('J') & 0x8000) players[1].keyPressed('j');
    if (GetAsyncKeyState('K') & 0x8000) players[1].keyPressed('k');
#endif
  bool is_fast_round = (round % 2 == 0);
    Point previousLocations[NUM_PLAYERS] = { players[0].getLocation(), players[1].getLocation() };
    players[0].move(is_fast_round);
    players[1].move(is_fast_round);

    for (int i = 0; i < NUM_PLAYERS; ++i) {
        if (!wallManager.isActive()) {
            continue;
        }
        if (wallManager.isWallCell(players[i].getLocation().getX(), players[i].getLocation().getY(), screen)) {
            int wallX = players[i].getLocation().getX();
            int wallY = players[i].getLocation().getY();
            // Both the owner (K collector) and the trapped player are blocked
            // from crossing the wall: revert to their previous position.
            players[i].erase();
            players[i].setLocation(previousLocations[i]);
            players[i].draw();
            if (wallManager.isActive() && wallManager.isWallCell(wallX, wallY, screen)) {
                gotoxy(wallX, wallY);
                set_color(Color::LightPurple);
                std::cout << '#';
                reset_color();
            }
        }
    }

    // Check if either player lost all lives
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (players[i].getLives() <= 0) {
            int winner = (i == 0) ? 1 : 0;
            announceWinner('A' + winner);
            current_status = GameStatus::MENU;
            return;
        }
    }

    // Spawn a new item at fixed interval
    itemSpawnCounter++;
    if (itemSpawnCounter >= ITEM_SPAWN_INTERVAL) {
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
            if (collected == 'K') {
                // if K collector is already inside the new wall area, lose a life
                // but keep current answer (do not create a new Player)
                if (wallManager.isInsideWallArea(
                        players[i].getLocation().getX(), players[i].getLocation().getY(),
                        players[opponent].getLocation().getX(), players[opponent].getLocation().getY())) {
                    players[i].loseLife();
                    players[i].erase();
                    Point respawn(i == 0 ? 10 : 70, 10, 0, 0, players[i].getChar());
                    players[i].setLocation(respawn);
                    players[i].draw();
                    wallManager.applyKWall(i, opponent, players, items, screen);
                }
            } else {
                ItemManager::applyItem(players[i], players[opponent], collected);
            }
        }
    }
   

    // Display player answers
    displayAnswers();

    // Show Player A info (bottom left)
    gotoxy(0, Screen::MAX_Y - 1);
    reset_color();
    std::cout << "A Score: " << players[0].getScore() << "  ";
    if (players[0].getLives() == 1) {
        set_color(Color::LightRed);
    }
    std::cout << "Lives: " << players[0].getLives();
    reset_color();
    std::cout << "   Speed Cycles: " << players[0].getSpeedCycles() << "      ";

    // Show Player B info (bottom right)
    gotoxy(Screen::MAX_X - 38, Screen::MAX_Y - 1);
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
    gotoxy(PAUSE_MSG_X, PAUSE_MSG_Y);
    std::cout << "Game paused, press ESC again to continue or H to go to the main menu" << std::endl;

    char key = get_single_char();
    switch (static_cast<PauseChoice>(key)) {
    case PAUSE_RESUME:
        current_status = GameStatus::PLAYING;
        gotoxy(PAUSE_MSG_X, PAUSE_MSG_Y);
        std::cout << "                                                                    " << std::endl;
        items.drawItems(); // Redraw items after pause
        wallManager.redraw(players, items, screen);
        players[0].draw();
        players[1].draw();
        break;
    case PAUSE_HOME_LOWER:
    case PAUSE_HOME_UPPER:
        current_status = GameStatus::MENU;
        break;
    default:
        break;
    }
}


void Game::displayAnswers() {
    gotoxy(0, 2);
    std::cout << "A Answer: " << players[0].getCurrentAnswer() << "               ";

    gotoxy(Screen::MAX_X - 18, 2);
    std::cout << "B Answer: " << players[1].getCurrentAnswer() << "               ";
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
        wallManager.reset(screen);
        exercise.generate(currentLevel, currentOperation);
        screen.draw();
    } else {
        char winner = (players[0].getScore() >= players[1].getScore()) ? 'A' : 'B';
        announceWinner(winner);
        current_status = GameStatus::MENU;
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