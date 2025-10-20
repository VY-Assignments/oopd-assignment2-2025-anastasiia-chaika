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
	void move(Direction direction);
	void shoot();
	void update_field();
private:
	std::unique_ptr<Field> field;
	std::unique_ptr<BotTank> bot_tank;
	std::unique_ptr<UserTank> user_tank;

};