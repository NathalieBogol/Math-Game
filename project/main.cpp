#include <iostream>
#include <iostream>
#include <ctime>   
#include <cstdlib> 
#include "console.h"
#include "Game.h"

int main() {
    // to reset the random
    srand(time(nullptr));

    init_console();
    clrscr();
    hideCursor();

    Game game;
    game.run();

    cleanup_console();
    clrscr();

    return 0;
}
