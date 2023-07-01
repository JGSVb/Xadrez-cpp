#include <cstdio>
#include "graphics.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv){
	#if 1
	chess::test();
	#endif

	chess::Game *game = new chess::Game;

	graphics::init();
	graphics::ChessWindow win{game};
	win.main();
	graphics::quit();

	delete game;

	return 0;
}
