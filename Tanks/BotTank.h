#pragma once
#include "Tank.h"
#include "Direction.h"
#include "Projectile.h"
#include <chrono>

class BotTank : public Tank{
public:
	BotTank();
	void move() override;
private:
	std::chrono::steady_clock::time_point last_move_time;
	std::chrono::milliseconds move_interval;
};
