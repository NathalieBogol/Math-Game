#include "Game.h"
#include <string>
#include "console.h"
#include "Items.h"
#include "ItemManager.h"
#include <iostream>
#include <ctime>

//Each of the players keys
static const char p_A_Keys[5] = { 'w', 'd', 'x', 'a', 's' };
static const char p_B_Keys[5] = { 'i', 'l', 'm', 'j', 'k' };

Game::Game(ProgramMode mode, bool silent) :
    current_status(GameStatus::MENU),
    fileManager(mode),
    silentMode(silent),
    itemSpawnCounter(0),
    roundNumber(0),
    players{
        Player(Point(10, 10, 0, 0, 'A'), p_A_Keys),
        Player(Point(70, 10, 0, 0, 'B'), p_B_Keys)
    }
{
    set_colors_enabled(colorsEnabled);
    if (fileManager.isLoading()) {
        ready = fileManager.prepareLoad();
        if (!ready) {
            runSummary = "Load failed: " + fileManager.getError();
        }
    }
}
//change name 
void Game::run() {
    if (!ready) {
        return;
    }

    if (fileManager.isLoading()) {
        currentLevel = fileManager.getLevel();
        currentOperation = fileManager.getOperation();
        reset_game();
        if (!ready) return;
        current_status = GameStatus::PLAYING;
    }

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
    finishFileRun();
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
    std::cout << "(" << (char)MENU_SELECT_GAME_MODE << ") Game mode [current: " << gameModeToString(currentGameMode) << "]";

    gotoxy(centerX - MENU_OPTION3_OFFSET, startY + 6);
    std::cout << "(" << (char)MENU_INSTRUCTIONS << ") Present instructions and keys";

    gotoxy(centerX - MENU_OPTION3_OFFSET, startY + 7);
    std::cout << "(" << (char)MENU_TOGGLE_COLORS << ") Colors Mode: " << (colorsEnabled ? "ON" : "OFF");

    gotoxy(centerX - MENU_OPTION4_OFFSET, startY + 8);
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


const char* Game::gameModeToString(GameMode gameMode) const {
    switch (gameMode) {
    case GameMode::HUMAN_VS_HUMAN:
        return "Human vs. Human";
    case GameMode::HUMAN_VS_COMPUTER:
        return "Human vs. Computer";
    case GameMode::COMPUTER_VS_COMPUTER:
        return "Computer vs. Computer";
    default:
        return "Human vs. Computer";
    }
}

void Game::selectGameMode() {
    switch (currentGameMode) {
    case GameMode::HUMAN_VS_COMPUTER:
        currentGameMode = GameMode::HUMAN_VS_HUMAN;
        break;
    case GameMode::HUMAN_VS_HUMAN:
        currentGameMode = GameMode::COMPUTER_VS_COMPUTER;
        break;
    case GameMode::COMPUTER_VS_COMPUTER:
        currentGameMode = GameMode::HUMAN_VS_COMPUTER;
        break;
    }
}

bool Game::isComputerPlayer(int playerIndex) const {
    if (currentGameMode == GameMode::COMPUTER_VS_COMPUTER) {
        return true;
    }
    return currentGameMode == GameMode::HUMAN_VS_COMPUTER && playerIndex == 1;
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
    case MENU_SELECT_GAME_MODE:
        selectGameMode();
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

    std::cout << "GAME MODES:" << std::endl;
    std::cout << "Human vs. Human, Human vs. Computer, or Computer vs. Computer" << std::endl;
    std::cout << "In Human vs. Computer, the human is always Player A on the left." << std::endl << std::endl;

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
    // Save starts a fresh seeded recording; load restores the seed from game.steps.
    gameTime = 0;
    gameEnded = false;
    itemSpawnCounter = 0;
    lastRecordedDirections[0] = Direction::STAY;
    lastRecordedDirections[1] = Direction::STAY;

    if (fileManager.isSaving()) {
        const unsigned int seed = static_cast<unsigned int>(std::time(nullptr)) + saveSeedCounter++;
        std::srand(seed);
        if (!fileManager.startSave(seed, currentLevel, currentOperation)) {
            ready = false;
            runSummary = "Save failed: " + fileManager.getError();
            current_status = GameStatus::EXIT;
            return;
        }
    } else if (fileManager.isLoading()) {
        std::srand(fileManager.getSeed());
    }

    players[0] = Player(Point(10, 10, 0, 0, 'A'), p_A_Keys);
    players[1] = Player(Point(70, 10, 0, 0, 'B'), p_B_Keys);
    roundNumber = 0;
    wallManager.reset(screen);
    exercise.generate(currentLevel, currentOperation); // Generate first exercise
    items.clearAll();
    screen.draw();
    items.ensureProgressItem(players[0], players[1],
        exercise.getCorrectAnswerString(), screen);
}

// Game loop
void Game::manage_playing() {
    gotoxy(0, 1); 
    std::cout << exercise.getExerciseString() << "                ";
    // create a useful item before the board can fill, without replacing items.
    items.ensureProgressItem(players[0], players[1],
        exercise.getCorrectAnswerString(), screen);
    if (!fileManager.isLoading()) {
        if (check_kbhit()) {
            char key = get_single_char();

            if (key == ESC) {
                current_status = GameStatus::PAUSED;
                return;
            }
            if (!isComputerPlayer(0)) players[0].keyPressed(key);
            if (!isComputerPlayer(1)) players[1].keyPressed(key);
        }

        // Solution to players not moving when keyboard on Hebrew. Used AI to find the solution.
#ifdef PLATFORM_WINDOWS
        if (!isComputerPlayer(0)) {
            if (GetAsyncKeyState('W') & 0x8000) players[0].keyPressed('w');
            if (GetAsyncKeyState('D') & 0x8000) players[0].keyPressed('d');
            if (GetAsyncKeyState('X') & 0x8000) players[0].keyPressed('x');
            if (GetAsyncKeyState('A') & 0x8000) players[0].keyPressed('a');
            if (GetAsyncKeyState('S') & 0x8000) players[0].keyPressed('s');
        }

        if (!isComputerPlayer(1)) {
            if (GetAsyncKeyState('I') & 0x8000) players[1].keyPressed('i');
            if (GetAsyncKeyState('L') & 0x8000) players[1].keyPressed('l');
            if (GetAsyncKeyState('M') & 0x8000) players[1].keyPressed('m');
            if (GetAsyncKeyState('J') & 0x8000) players[1].keyPressed('j');
            if (GetAsyncKeyState('K') & 0x8000) players[1].keyPressed('k');
        }
#endif

        for (int i = 0; i < NUM_PLAYERS; ++i) {
            if (isComputerPlayer(i)) {
                players[i].setDirection(computerController.chooseDirection(
                    players[i], exercise, items, screen));
            }
        }
    }

    ++gameTime;
    if (fileManager.isLoading()) {
        fileManager.applyDirections(gameTime, players);
    } else {
        recordDirectionChanges();
    }
    wallManager.tick(screen);

    bool is_fast_round = (gameTime % 2 == 0);
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

        if (collected != ' ') {
            fileManager.recordPickup(gameTime, i, collected);
        }
        int previousLives[NUM_PLAYERS] = {
            players[0].getLives(), players[1].getLives()
        };
        int previousScores[NUM_PLAYERS] = {
            players[0].getScore(), players[1].getScore()
        };

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
                    players[i].draw();;
                }
                wallManager.applyKWall(i, opponent, players, items, screen);
            } else {
                ItemManager::applyItem(players[i], players[opponent], collected);
            }
        }
        if (collected != ' ') {
            recordPlayerChanges(previousLives, previousScores);
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

    if (fileManager.isLoading() && !gameEnded
        && gameTime >= fileManager.getExpectedEndTime()) {
        current_status = GameStatus::EXIT;
    }

    sleepFor(50, 20);
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
            fileManager.recordScore(gameTime, i, POINTS_PER_SOLUTION);
            gotoxy(25, 12);
        set_color(Color::LightYellow);
        std::cout << "Player " << (char)('A' + i) << " solved it!";
        reset_color();
            sleepFor(1500, 600);
        }
        if (players[i].getScore() >= WINNING_SCORE) {
            announceWinner('A' + i);
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
        items.ensureProgressItem(players[0], players[1],
            exercise.getCorrectAnswerString(), screen);
    } else {
        char winner = (players[0].getScore() >= players[1].getScore()) ? 'A' : 'B';
        announceWinner(winner);
    }
}

void Game::announceWinner(char winnerChar) {
    const int winnerIndex = winnerChar - 'A';
    fileManager.recordEnd(gameTime, winnerIndex);
    gameEnded = true;
    if (!silentMode) clrscr();
    gotoxy(25, 12);
    set_color(Color::LightYellow);
    if (winnerChar == 'A') {
        std::cout << "*** PLAYER A WINS! ***";
    } else {
        std::cout << "*** PLAYER B WINS! ***";
    }
    reset_color();
    sleepFor(2000, 800);
    current_status = fileManager.isLoading() ? GameStatus::EXIT : GameStatus::MENU;
}

void Game::recordDirectionChanges() {
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        const Direction currentDirection = players[i].getDirection();
        if (currentDirection != lastRecordedDirections[i]) {
            fileManager.recordDirection(gameTime, i, currentDirection);
            lastRecordedDirections[i] = currentDirection;
        }
    }
}

void Game::recordPlayerChanges(const int previousLives[NUM_PLAYERS],
    const int previousScores[NUM_PLAYERS]) {
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        if (players[i].getLives() < previousLives[i]) {
            fileManager.recordLifeLoss(gameTime, i);
        }
        if (players[i].getScore() > previousScores[i]) {
            fileManager.recordScore(gameTime, i,
                players[i].getScore() - previousScores[i]);
        }
    }
}

void Game::sleepFor(int normalMilliseconds, int loadMilliseconds) const {
    if (silentMode) {
        return;
    }
    sleep_ms(fileManager.isLoading() ? loadMilliseconds : normalMilliseconds);
}

void Game::finishFileRun() {
    fileManager.closeFiles();
    if (fileManager.isLoading()) {
        fileManager.compareResults(runSummary);
    }
}
