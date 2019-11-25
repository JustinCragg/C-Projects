#include "Game.h"

int main() {

    Game* game = new Game(1280, 720);

    game->run("Game", game->m_screenWidth, game->m_screenHeight, false);

    delete game;
}