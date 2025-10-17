#include "Field.h"
#include <random>
#include <ctime>
#include <iostream>

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

std::vector<std::vector<char>> Field::init_field() {
	std::vector<std::vector<char>> field_grid(this->rows, std::vector<char>(this->cols, ' '));

	int totalCells = rows * cols;
	int number_of_obstacles = static_cast<int>(coeficient * totalCells);

	field_grid[bot_row][bot_col] = 'B';
	field_grid[us_row][us_col] = 'T';

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
	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			std::cout << field_grid[i][j];
		}
		std::cout << '\n';
	}
}


//fix checking!
void Field::clear_tanks_positions() {
	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			if (field_grid[i][j] == 'B' || field_grid[i][j] == 'T' || field_grid[i][j] == '^' || field_grid[i][j] == 'v' || field_grid[i][j] == '<' || field_grid[i][j] == '>') {
				field_grid[i][j] = ' ';
			}
		}
	}
}

void Field::update_field() {
	clear_tanks_positions();
	
	field_grid[bot_row][bot_col] = 'B';
	field_grid[us_row][us_col] = 'T';

}