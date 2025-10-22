#include "GameEngine.h"
#include "Direction.h"
#include <conio.h>
#include <thread>
#include <chrono>
#include <windows.h>

int main() {
	
	//temporary solution to hide cursor while not using sfml
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);

	GameEngine ge;
	ge.display_field();

	while (true) {
		if (ge.isGameOver()) return 0;
		ge.update_field();
		ge.display_field();

		if (_kbhit()) {
			int key = _getch();

			if (key == 27) {
				break;
			}

			else if (key == 224) {
				int key_direction = _getch();
				Direction d = Direction::NODIRECTION;
				switch (key_direction) {
				case 72:							//up
					d = Direction::UP;
					break;
				case 80:							//down
					d = Direction::DOWN;
					break;
				case 75:							//left
					d = Direction::LEFT;
					break;
				case 77:							//right
					d = Direction::RIGHT;
					break;
				}
				if (d != Direction::NODIRECTION) {
					ge.move_user_tank(d);
				}
			}

			else if (key == 32) {
				ge.user_shoot();
			}

		}

	}

	return 0;
}