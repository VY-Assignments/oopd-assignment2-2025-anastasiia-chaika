#include "GameEngine.h"
#include "SFMLRenderer.h"
#include "Direction.h"
#include <windows.h>
#include <memory>

int main() {

#ifdef SFML_RENDER
	std::unique_ptr<IGameEngine> ge = IGameEngine::create_game_engine();
	SFMLRenderer rend(std::move(ge));
	rend.mainLoop();
#else
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);

	std::unique_ptr<IGameEngine> ge = IGameEngine::create_game_engine();
	ge->render();
#endif

	return 0;
}

