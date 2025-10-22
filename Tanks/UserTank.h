#pragma once
#include "Tank.h"
#include "Projectile.h"

class UserTank : public Tank {
public:
	UserTank();
	void move() override;
};
