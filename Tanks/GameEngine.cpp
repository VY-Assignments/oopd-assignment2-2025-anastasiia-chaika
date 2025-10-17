#include "GameEngine.h"
#include <vector>
#include <iostream>

GameEngine::GameEngine() {
	this->field = std::make_unique<Field>();
	this->bot_tank = std::make_unique<BotTank>();
	this->user_tank = std::make_unique<UserTank>();
}

const void GameEngine::display_field() {
	system("cls");
	this->field->display_field();
}

void GameEngine::move(Direction direction) {			//if cell available and outside the board
	if (direction != Direction::NODIRECTION) {
		if (user_tank->get_direction() == direction) {
			int row = user_tank->get_row_pos();
			int col = user_tank->get_col_pos();
			switch (direction) {
			case UP:
				user_tank->set_row(row - 1);
				break;
			case DOWN:
				user_tank->set_row(row + 1);
				break;
			case LEFT:
				user_tank->set_col(col - 1);
				break;
			case RIGHT:
				user_tank->set_col(col + 1);
				break;
			}

			field->set_us_row(user_tank->get_row_pos());
			field->set_us_col(user_tank->get_col_pos());
		}
		else {
			user_tank->set_direction(direction);
		}
	}

}

void GameEngine::shoot() {

}

void GameEngine::update_field() {
	this->field->update_field();
}
