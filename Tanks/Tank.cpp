#include "Tank.h"
#include "Projectile.h"
#include <memory>
#include <iostream>

std::unique_ptr<Projectile> Tank::shoot() {
	Direction direct = this->get_direction();
	std::unique_ptr<Projectile> proj = std::make_unique<Projectile>(direct);
	int r = this->get_row_pos();
	int c = this->get_col_pos();

	switch (direct) {
	case UP:
		r--;
		break;
	case DOWN:
		r++;
		break;
	case LEFT:
		c--;
		break;
	case RIGHT:
		c++;
		break;
	}

	proj->set_row(r);
	proj->set_col(c);

	return proj;
}

int Tank::get_hp() {
	return hp;
}

void Tank::lower_hp() {
	hp = hp-20;
}

bool Tank::get_is_shoot() {
	return is_shot;
}

void Tank::set_condition(bool c) {
	is_shot = c;
}