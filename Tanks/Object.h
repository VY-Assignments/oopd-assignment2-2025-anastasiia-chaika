#pragma once
#include "Direction.h"

class Object {
public:
	int get_col_pos() const;
	int get_row_pos() const;
	Direction get_direction() const;
	void set_direction(Direction direction);
	virtual void move() = 0;
protected:
	int col_pos = 0;
	int row_pos = 0;
	Direction d = Direction::NODIRECTION;

	void update_coordinates();

	void set_row(int new_row);
	void set_col(int new_col);
};
