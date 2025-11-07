#include "Field.h"
#include "Projectile.h"
#include "BotTank.h"
#include "UserTank.h"
#include "Object.h"
#include "CellType.h"
#include <random>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>

Field::Field() {
	init_field();
}

void Field::set_us_row(int new_row) {
	us_row = new_row;
}

void Field::set_us_col(int new_col) {
	us_col = new_col;
}

void Field::set_us_hp(int hp) {
	user_hp = hp;
}

void Field::init_field() {
	field_grid = std::vector<std::vector<CellType>>(this->rows, std::vector<CellType>(this->cols, CellType::EMPTY));

	int totalCells = rows * cols;
	int number_of_obstacles = static_cast<int>(coeficient * totalCells);

	field_grid[us_row][us_col] = CellType::USERTANK;

	std::mt19937 rng(time(nullptr));
	std::uniform_int_distribution<int> dist_rows(0, rows - 1);
	std::uniform_int_distribution<int> dist_cols(0, cols - 1);

	int placed = 0;

	while (placed < number_of_obstacles) {
		int r = dist_rows(rng);
		int c = dist_cols(rng);

		if (field_grid[r][c] == CellType::EMPTY) {
			field_grid[r][c] = CellType::WALL;
		}
		placed++;
	}
}

void Field::display_field(const std::vector<std::unique_ptr<BotTank>>& bot_tanks) const {
	std::cout << "User HP: " << user_hp << std::endl;
	for (int i = 0;i < bot_tanks.size();i++) {
		std::cout << "Bot  HP: " << bot_tanks[i]->get_hp() << std::endl;
	}

	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			CellType c = field_grid[i][j];
			switch (c) {
			case CellType::WALL:
				std::cout << '#';
				break;
			case CellType::PROJECTILE:
				std::cout << '*';
				break;
			case CellType::BOTTANK:
				std::cout << 'B';
				break;
			case CellType::USERTANK:
				std::cout << 'U';
				break;
			case CellType::EMPTY:
				std::cout << ' ';
			}
		}
		std::cout << '\n';
	}
}

void Field::clear_objects_positions() {
	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			if (field_grid[i][j] == CellType::BOTTANK || field_grid[i][j] == CellType::USERTANK || field_grid[i][j] == CellType::PROJECTILE) {
				field_grid[i][j] = CellType::EMPTY;
			}
		}
	}
}

void Field::update_field(const std::vector<std::unique_ptr<Projectile>>& projectiles, const std::vector<std::unique_ptr<BotTank>>& bot_tanks) {
	clear_objects_positions();
	
	field_grid[us_row][us_col] = CellType::USERTANK;
	
	for (const auto& bot : bot_tanks) {
		field_grid[bot->get_row_pos()][bot->get_col_pos()] = CellType::BOTTANK;
	}

	for (const auto& p : projectiles) {
		field_grid[p->get_row_pos()][p->get_col_pos()] = CellType::PROJECTILE;
	}
}

bool Field::cell_is_free(int r, int c) const {
	if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
	return !(field_grid[r][c] == CellType::USERTANK || field_grid[r][c] == CellType::BOTTANK || field_grid[r][c] == CellType::WALL);
}

const std::vector<std::vector<CellType>>& Field::get_field() const{
	return field_grid;
}