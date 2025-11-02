#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Direction.h"

class IGameEngine {
public:
	virtual ~IGameEngine() = default;
	virtual void render() =0;

	static std::unique_ptr<IGameEngine> create_game_engine();

	virtual std::string isGameOver()=0;
	virtual const std::vector<std::vector<char>>& get_field() const=0;

	virtual void move_user_tank(Direction direction)=0;
	virtual void user_shoot()=0;
	virtual bool bot_shoot()=0;
	virtual void update_field()=0;

	virtual int get_user_hp()=0;
	virtual int get_bot_hp()=0;

};