#pragma once
#include "Object.h"
#include "Projectile.h"
#include <memory>
#include <chrono>

class Tank : public Object {
public:
	int get_hp();
	std::unique_ptr<Projectile> shoot();
	void lower_hp();
	bool get_is_shoot();
	void set_condition(bool condition);

	std::chrono::steady_clock::time_point shoot_time;
protected:
	int hp=100;
	bool is_shot=false;
};