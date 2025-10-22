#pragma once
#include "Object.h"
#include "Projectile.h"
#include <memory>

class Tank : public Object {
public:
	int get_hp();
	std::unique_ptr<Projectile> shoot();
	void lower_hp();
protected:
	int hp=100;
};