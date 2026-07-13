#pragma once
#include "Player.h"
#include "Screen.h"
#include "Items.h"
#include "MathExercise.h"
#include "Level.h"
#include "Operation.h"
#include "WallManager.h"
#include "ComputerController.h"
#include "GameFileManager.h"

enum class GameStatus {
	MENU,
	INSTRUCTIONS,
	PLAYING,
	PAUSED,
	EXIT
};

enum KeyCode {
	ESC = 27
};

enum class GameMode {
	HUMAN_VS_COMPUTER,
	HUMAN_VS_HUMAN,
	COMPUTER_VS_COMPUTER
};

// Menu choices as char-backed enum (use char values to match input)
enum MenuChoice {
	MENU_START = '1',
	MENU_SELECT_LEVEL = '2',
   MENU_SELECT_OPERATION = '3',
	MENU_SELECT_GAME_MODE = '4',
	MENU_TOGGLE_COLORS = '7',
	MENU_INSTRUCTIONS = '8',
	MENU_EXIT = '9'
};

// Pause choices grouped
enum PauseChoice {
	PAUSE_RESUME = ESC, // resume on ESC
	PAUSE_HOME_LOWER = 'h',
	PAUSE_HOME_UPPER = 'H'
};

class Game {
private:
	static constexpr int NUM_PLAYERS = 2;
	Screen screen;
	Player players[NUM_PLAYERS]; // players[0] = A, players[1] = B
	Items items;
	MathExercise exercise;
	GameStatus current_status;
	Level currentLevel = Level::EASY;
	Operation currentOperation = Operation::ADD;
	GameMode currentGameMode = GameMode::HUMAN_VS_COMPUTER;
	ComputerController computerController;
	GameFileManager fileManager;
	bool silentMode = false;
	bool ready = true;
	bool gameEnded = false;
	std::size_t gameTime = 0;
	unsigned int saveSeedCounter = 0;
	Direction lastRecordedDirections[NUM_PLAYERS] = { Direction::STAY, Direction::STAY };
	std::string runSummary;
	int itemSpawnCounter = 0; // Counter for item spawn timing
	int roundNumber = 0; // Current round (0-2 for 3 rounds)
	static constexpr int TOTAL_ROUNDS = 3;
	static constexpr int POINTS_PER_SOLUTION = 10;
	static constexpr int POINTS_FOR_DOLLAR = 5;
	static constexpr int WINNING_SCORE = 30; // 3 rounds * 10 points
	static constexpr int MENU_CENTER_X = 80 / 2;
	static constexpr int MENU_START_Y = 25 / 2 - 4;
	static constexpr int MENU_TITLE_OFFSET = 8;
	static constexpr int MENU_OPTION1_OFFSET = 9;
	static constexpr int MENU_OPTION2_OFFSET = 14;
	static constexpr int MENU_OPTION3_OFFSET = 11;
	static constexpr int MENU_OPTION4_OFFSET = 5;
	static constexpr int ITEM_SPAWN_INTERVAL = 20;

	static constexpr int PAUSE_MSG_X = 10;
	static constexpr int PAUSE_MSG_Y = 12;
	bool colorsEnabled = true;
    WallManager wallManager;

	void manage_menu();
	void manage_instructions();
	void manage_playing();
	void manage_pause();

	void reset_game();
	void draw_menu();
	void selectLevel();
	const char* levelToString(Level level);
	void selectOperation();
	const char* operationToString(Operation operation);
	void selectGameMode();
	const char* gameModeToString(GameMode gameMode) const;
	bool isComputerPlayer(int playerIndex) const;

	void check_status();
	void displayAnswers();
	void nextRound();
	void announceWinner(char winnerChar);
	void recordDirectionChanges();
	void recordPlayerChanges(const int previousLives[NUM_PLAYERS],
		const int previousScores[NUM_PLAYERS]);
	void sleepFor(int normalMilliseconds, int loadMilliseconds) const;
	void finishFileRun();

public:
	Game(ProgramMode mode = ProgramMode::NORMAL, bool silent = false);
	void run();
	bool isReady() const { return ready; }
	const std::string& getRunSummary() const { return runSummary; }

};
