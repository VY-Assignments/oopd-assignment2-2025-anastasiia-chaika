#pragma once
#include <vector>
#include "Direction.h"

class Field {
public:
	Field();
	void display_field() const;
	std::vector<std::vector<char>> init_field();
	void update_field();

	int get_bot_row() const;
	int get_bot_col() const;
	int get_us_row() const;
	int get_us_col() const;

	void set_us_row(int new_row);
	void set_us_col(int new_col);
	void set_us_direction(Direction direction);

	bool cell_is_free(int r, int c) const;
private:
	int cols = 20;
	int rows = 20;

	int bot_row = 1;
	int bot_col = 1;
	int us_row = 18;
	int us_col = 18;

	Direction us_dir = Direction::UP;

	double coeficient = 0.03;
	std::vector<std::vector<char>> field_grid;
	void clear_tanks_positions();
};