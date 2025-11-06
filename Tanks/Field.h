#pragma once
#include <vector>
#include "Direction.h"
#include "Projectile.h"
#include "BotTank.h"
#include "CellType.h"
#include <memory>

class Field {
public:
	Field();
	void display_field(const std::vector<std::unique_ptr<BotTank>>& bot_tanks) const;
	void init_field();
	void update_field(const std::vector<std::unique_ptr<Projectile>>& projectiles, const std::vector<std::unique_ptr<BotTank>>& bot_tanks);
	const std::vector<std::vector<CellType>>& get_field() const;

	void set_us_row(int new_row);
	void set_us_col(int new_col);

	void set_us_hp(int h);

	bool cell_is_free(int r, int c) const;
private:
	int cols = 20;
	int rows = 20;

	int us_row = 18;
	int us_col = 18;

	int user_hp = 100;

	double coeficient = 0.03;
	std::vector<std::vector<CellType>> field_grid;
	void clear_objects_positions();
};