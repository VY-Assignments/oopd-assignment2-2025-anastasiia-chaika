#include "Object.h"
#include "Direction.h"

int Object::get_col_pos() const{
	return col_pos;
}

int Object::get_row_pos() const{
	return row_pos;
}

Direction Object::get_direction() const {
	return d;
}

void Object::set_row(int new_row) {
	row_pos = new_row;
}

void Object::set_col(int new_col) {
	col_pos = new_col;
}

void Object::set_direction(Direction direction) {
	d = direction;
}