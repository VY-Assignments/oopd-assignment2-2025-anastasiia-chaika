#pragma once
#include "Object.h"
#include "Direction.h"
#include <chrono>

class Projectile : public Object {
public:
	Projectile(Direction direction);
	void move() override;
private:
	std::chrono::steady_clock::time_point last_move_time;
	std::chrono::milliseconds move_interval;
};