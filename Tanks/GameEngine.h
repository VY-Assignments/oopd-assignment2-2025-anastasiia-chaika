#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Direction.h"
#include "Projectile.h"
#include "GameFinished.h"
#include "BotTank.h"
#include "GameMode.h"
#include "CellType.h"

class IGameEngine {
public:
	virtual ~IGameEngine() = default;
	/*virtual void render() =0;*/

	static std::unique_ptr<IGameEngine> create_game_engine();

	virtual GameFinished isGameOver() =0;
	virtual const std::vector<std::vector<CellType>>& get_field() const=0;

	virtual void move_user_tank(Direction direction)=0;
	virtual void user_shoot()=0;
	virtual bool bot_shoot()=0;
	virtual void update_field()=0;

	virtual int get_user_hp()=0;
	virtual int get_bot_hp(int index)=0;

	virtual bool user_is_shot() = 0;
	virtual bool bot_is_shot(int index) = 0;

	virtual const std::vector<std::unique_ptr<Projectile>>& get_projectiles() const=0;
	virtual Direction get_user_direction() = 0;
	virtual Direction get_bot_direction(int index) = 0;

	virtual const std::vector<std::unique_ptr<BotTank>>& get_bot_tanks() const = 0;

	virtual void start_game(GameMode m)=0;

};