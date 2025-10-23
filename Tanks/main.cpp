#include "GameEngine.h"
#include "Direction.h"
#include <windows.h>

int main() {
	
	//temporary solution to hide cursor while not using sfml
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);

	GameEngine ge;
	ge.run();

	return 0;
}