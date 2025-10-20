#include "BotTank.h"

BotTank::BotTank() : Tank (){
	row_pos = 1;
	col_pos = 1;
	d = Direction::DOWN;

	move_interval = std::chrono::milliseconds(500);
	last_move_time = std::chrono::steady_clock::now();
}

void BotTank::update() {
	auto now = std::chrono::steady_clock::now();
	if (now - last_move_time >= move_interval) {
		bot_move();
		last_move_time = now;
	}
}

void BotTank::bot_move() {
	switch (d) {
	case UP:
		row_pos--;
		break;
	case DOWN:
		row_pos++;
		break;
	case RIGHT:
		col_pos++;
		break;
	case LEFT:
		col_pos--;
		break;
	}

}
