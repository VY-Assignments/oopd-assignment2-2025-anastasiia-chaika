#pragma once
#include "Tank.h"

class UserTank : public Tank {
public:
	UserTank();
	void move() override;
};
