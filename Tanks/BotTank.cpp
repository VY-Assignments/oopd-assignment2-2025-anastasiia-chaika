#include "BotTank.h"

BotTank::BotTank() : Tank (){
	row_pos = 1;
	col_pos = 1;
	d = Direction::DOWN;

	move_interval = std::chrono::milliseconds(500);
	last_move_time = std::chrono::steady_clock::now();

	shoot_interval = std::chrono::milliseconds(1000);
	last_shoot_time = std::chrono::steady_clock::now();
}

BotTank::BotTank(int row, int col) {
	row_pos = row;
	col_pos = col;
	d = Direction::RIGHT;

	move_interval = std::chrono::milliseconds(500);
	last_move_time = std::chrono::steady_clock::now();

	shoot_interval = std::chrono::milliseconds(1000);
	last_shoot_time = std::chrono::steady_clock::now();
}

void BotTank::move() {
	auto now = std::chrono::steady_clock::now();
	if (now - last_move_time >= move_interval) {
		update_coordinates();
		last_move_time = now;
	}
}

std::unique_ptr<Projectile> BotTank::bot_shoot() {
	auto now = std::chrono::steady_clock::now();
	if (now - last_shoot_time >= shoot_interval) {
		std::unique_ptr<Projectile> pr = shoot();
		last_shoot_time = now;
		return pr;
	}
	else return nullptr;
}
