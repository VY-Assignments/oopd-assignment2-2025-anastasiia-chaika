#include "BotTank.h"

BotTank::BotTank() : Tank (){
	this->row_pos = 1;
	this->col_pos = 1;
	this->d = Direction::DOWN;
}