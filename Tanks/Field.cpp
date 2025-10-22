#include "Field.h"
#include "Projectile.h"
#include <random>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>

Field::Field() {
	field_grid = init_field();
}


int Field::get_bot_row() const {
	return bot_row;
}

int Field::get_bot_col() const {
	return bot_col;
}

int Field::get_us_row() const {
	return us_row;
}

int Field::get_us_col() const {
	return us_col;
}

void Field::set_us_row(int new_row) {
	us_row = new_row;
}

void Field::set_us_col(int new_col) {
	us_col = new_col;
}

void Field::set_bot_row(int new_row) {
	bot_row = new_row;
}

void Field::set_bot_col(int new_col) {
	bot_col = new_col;
}

void Field::set_us_direction(Direction direction) {
	us_dir = direction;
}

void Field::set_bot_direction(Direction direction) {
	bot_dir = direction;
}

void Field::set_bot_hp(int hp) {
	bot_hp = hp;
}

void Field::set_us_hp(int hp) {
	user_hp = hp;
}

std::vector<std::vector<char>> Field::init_field() {
	std::vector<std::vector<char>> field_grid(this->rows, std::vector<char>(this->cols, ' '));

	int totalCells = rows * cols;
	int number_of_obstacles = static_cast<int>(coeficient * totalCells);

	field_grid[bot_row][bot_col] = 'B';
	switch (us_dir) {
	case UP:
		field_grid[us_row][us_col] = '^';
		break;
	case DOWN:
		field_grid[us_row][us_col] = 'v';
		break;
	case RIGHT:
		field_grid[us_row][us_col] = '>';
		break;
	case LEFT:
		field_grid[us_row][us_col] = '<';
		break;
	}

	std::mt19937 rng(time(nullptr));
	std::uniform_int_distribution<int> dist_rows(0, rows - 1);
	std::uniform_int_distribution<int> dist_cols(0, cols - 1);

	int placed = 0;

	while (placed < number_of_obstacles) {
		int r = dist_rows(rng);
		int c = dist_cols(rng);

		if (field_grid[r][c] == ' ') {
			field_grid[r][c] = '#';
		}
		placed++;
	}

	return field_grid;
}

void Field::display_field() const {
	std::cout << "User HP: " << user_hp << std::endl;
	std::cout << "Bot  HP: " << bot_hp << std::endl;

	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			std::cout << field_grid[i][j];
		}
		std::cout << '\n';
	}
}

void Field::clear_objects_positions() {
	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			if (field_grid[i][j] == 'B' || field_grid[i][j] == '^' || field_grid[i][j] == 'v' || field_grid[i][j] == '<' || field_grid[i][j] == '>' || field_grid[i][j] == '*') {
				field_grid[i][j] = ' ';
			}
		}
	}
}

void Field::update_field(const std::vector<std::unique_ptr<Projectile>>& projectiles) {
	clear_objects_positions();
	
	switch (us_dir) {
	case UP:
		field_grid[us_row][us_col] = '^';
		break;
	case DOWN:
		field_grid[us_row][us_col] = 'v';
		break;
	case RIGHT:
		field_grid[us_row][us_col] = '>';
		break;
	case LEFT:
		field_grid[us_row][us_col] = '<';
		break;
	}
	field_grid[bot_row][bot_col] = 'B';

	for (const auto& p : projectiles) {
		field_grid[p->get_row_pos()][p->get_col_pos()] = '*';
	}
}

bool Field::cell_is_free(int r, int c) const {
	if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
	return !(field_grid[r][c] == '#' || field_grid[r][c] == 'B' || field_grid[r][c] == '^' || field_grid[r][c] == '>' || field_grid[r][c] == 'v' || field_grid[r][c] == '<');
}