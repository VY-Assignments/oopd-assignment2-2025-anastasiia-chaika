#include "UserTank.h"

UserTank::UserTank() : Tank() {
	this->row_pos = 18;
	this->col_pos = 18;
	this->d = Direction::UP;
}