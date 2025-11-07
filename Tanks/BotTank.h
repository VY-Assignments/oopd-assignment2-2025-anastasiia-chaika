#pragma once
#include "Tank.h"
#include "Direction.h"
#include "Projectile.h"
#include <chrono>
#include <memory>

class BotTank : public Tank{
public:
	BotTank();
	BotTank(int row, int col);
	void move() override;
	std::unique_ptr<Projectile> bot_shoot();
private:
	std::chrono::steady_clock::time_point last_move_time;
	std::chrono::milliseconds move_interval;

	std::chrono::steady_clock::time_point last_shoot_time;
	std::chrono::milliseconds shoot_interval;
};
