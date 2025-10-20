#pragma once
#include "Tank.h"
#include "Direction.h"
#include <chrono>

class BotTank : public Tank{
public:
	BotTank();
	void bot_move();
	void update();
private:
	std::chrono::steady_clock::time_point last_move_time;
	std::chrono::milliseconds move_interval;
};
