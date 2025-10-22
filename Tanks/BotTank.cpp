#include "BotTank.h"

BotTank::BotTank() : Tank (){
	row_pos = 1;
	col_pos = 1;
	d = Direction::DOWN;

	move_interval = std::chrono::milliseconds(500);
	last_move_time = std::chrono::steady_clock::now();
}

void BotTank::move() {
	auto now = std::chrono::steady_clock::now();
	if (now - last_move_time >= move_interval) {
		update_coordinates();
		last_move_time = now;
	}
}
