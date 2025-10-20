#include "GameEngine.h"
#include <vector>
#include <iostream>

GameEngine::GameEngine() {
	field = std::make_unique<Field>();
	bot_tank = std::make_unique<BotTank>();
	user_tank = std::make_unique<UserTank>();
}

const void GameEngine::display_field() {
	system("cls");
	field->display_field();
}

void GameEngine::move(Direction direction) {			//if cell available and outside the board
	if (direction != Direction::NODIRECTION) {
		if (user_tank->get_direction() == direction) {
			int row = user_tank->get_row_pos();
			int col = user_tank->get_col_pos();
			switch (direction) {
			case UP:
				if (!field->cell_is_free(row - 1, col)) return;
				user_tank->set_row(row - 1);
				break;
			case DOWN:
				if (!field->cell_is_free(row + 1, col)) return;
				user_tank->set_row(row + 1);
				break;
			case LEFT:
				if (!field->cell_is_free(row, col - 1)) return;
				user_tank->set_col(col - 1);
				break;
			case RIGHT:
				if (!field->cell_is_free(row, col + 1)) return;
				user_tank->set_col(col + 1);
				break;
			}
		}
		else {
			user_tank->set_direction(direction);
		}

		field->set_us_row(user_tank->get_row_pos());
		field->set_us_col(user_tank->get_col_pos());
		field->set_us_direction(user_tank->get_direction());
	}

}

void GameEngine::shoot() {

}

void GameEngine::update_field() {
	this->field->update_field();
}
