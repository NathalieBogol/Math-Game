#pragma once
#include "Player.h"
#include "Screen.h"
#include "Item.h"
#include "MathExercise.h"

enum class GameStatus {
	MENU,
	INSTRUCTIONS,
	PLAYING,
	PAUSED,
	EXIT
};

class Game {
private:
	Screen screen;
	Player player_A;
	Player player_B;
	Item items;
	MathExercise exercise;
	GameStatus current_status;
	int itemSpawnCounter = 0; // Counter for item spawn timing
	int roundNumber = 0; // Current round (0-2 for 3 rounds)
	static constexpr int TOTAL_ROUNDS = 3;
	static constexpr int POINTS_PER_SOLUTION = 10;
	static constexpr int POINTS_FOR_DOLLAR = 5;
	static constexpr int WINNING_SCORE = 30; // 3 rounds * 10 points

	void manage_menu();
	void manage_instructions();
	void manage_playing();
	void manage_pause();

	void reset_game();
	void draw_menu();

	void update_text();
	void manage_collision();
	void check_status();
	void processSpecialItem(Player& current, Player& opponent, char itemChar);
	void displayAnswers();
	void nextRound();
	void announceWinner(char winnerChar);

public:
	Game();
	void run();

};