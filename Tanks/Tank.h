#pragma once
#include "Object.h"
#include "Projectile.h"
#include <memory>

class Tank : public Object {
public:
	int get_hp();
	std::unique_ptr<Projectile> shoot();
protected:
	int hp=100;
};