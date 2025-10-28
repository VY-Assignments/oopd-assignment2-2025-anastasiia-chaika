#pragma once
#include <vector>
#include <string>
#include "Field.h"
#include "Direction.h"
#include "BotTank.h"
#include "UserTank.h"
#include "Projectile.h"
#include "Keys.h"

class IGameEngine {
public:
	IGameEngine() = default;
	virtual ~IGameEngine() = default;
	virtual int run() = 0;

	static std::shared_ptr<IGameEngine> create_game_engine();

};