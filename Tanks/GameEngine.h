#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Field.h"
#include "Direction.h"
#include "BotTank.h"
#include "UserTank.h"

class GameEngine {
public:
	GameEngine();
	const void display_field();
	void move_user_tank(Direction direction);
	void shoot();
	void update_field();
	void move_bot_tank();
private:
	std::unique_ptr<Field> field;
	std::unique_ptr<BotTank> bot_tank;
	std::unique_ptr<UserTank> user_tank;

};