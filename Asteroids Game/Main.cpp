#include "Game.h"

#include <conio.h>
#include <iostream>

int main() {
    int diff = -1;
    while (diff < 0 || diff > 9) {
        std::cout << "Select your difficulty (0-9)" << std::endl;

        diff = _getch();
        if (diff == 27) {
            return 0;
        }
        if (isdigit(diff)) {
            diff -= '0';
            std::cout << diff << std::endl << std::endl;
        }
    }

	auto game = new Game(1280, 720, diff);

	// initialise and loop
	game->run("Game", game->m_screenWidth, game->m_screenHeight, false);

    std::cout << "GAME OVER" << std::endl << std::endl;
    system("pause");

	delete game;
}