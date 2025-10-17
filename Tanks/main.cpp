#include "GameEngine.h"
#include "Direction.h"
#include <conio.h>

int main() {
	GameEngine ge;
	ge.display_field();

	while (true) {
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
					ge.move(d);
				}
			}

			else if (key == 32) {
				ge.shoot();
			}

			ge.update_field();
			ge.display_field();
		}
	}

	return 0;
}