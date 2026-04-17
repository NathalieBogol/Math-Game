#pragma once
#include "Player.h"
#include "Screen.h"

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
	GameStatus current_status;

	void manage_menu();
	void manage_instructions();
	void manage_playing();
	void manage_pause();

	void reset_game();
	void draw_menu();

	void update_text();
	void manage_collision();
	void check_status();

public:
	Game();
	void run();

};