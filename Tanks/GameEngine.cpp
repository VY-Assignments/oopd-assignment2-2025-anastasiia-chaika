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

void GameEngine::move_bot_tank() {
	Direction direction = bot_tank->get_direction();
	if (direction != Direction::NODIRECTION) {
		int row = bot_tank->get_row_pos();
		int col = bot_tank->get_col_pos();
		switch (direction) {
		case UP:
			if (!field->cell_is_free(row - 1, col)) {
				bot_tank->set_direction(LEFT);
			}
			bot_tank->move();
			break;
		case DOWN:
			if (!field->cell_is_free(row + 1, col)) {
				bot_tank->set_direction(RIGHT);
			}
			bot_tank->move();
			break;
		case LEFT:
			if (!field->cell_is_free(row, col - 1)) {
				bot_tank->set_direction(DOWN);
			}
			bot_tank->move();
			break;
		case RIGHT:
			if (!field->cell_is_free(row, col + 1)) {
				bot_tank->set_direction(UP);
			}
			bot_tank->move();
			break;
		}
	}
}

void GameEngine::move_user_tank(Direction direction) {
	if (direction != Direction::NODIRECTION) {
		if (user_tank->get_direction() == direction) {
			int row = user_tank->get_row_pos();
			int col = user_tank->get_col_pos();
			switch (direction) {
			case UP:
				if (!field->cell_is_free(row - 1, col)) return;
				user_tank->move();
				break;
			case DOWN:
				if (!field->cell_is_free(row + 1, col)) return;
				user_tank->move();
				break;
			case LEFT:
				if (!field->cell_is_free(row, col - 1)) return;
				user_tank->move();
				break;
			case RIGHT:
				if (!field->cell_is_free(row, col + 1)) return;
				user_tank->move();
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
	move_bot_tank();
	field->set_bot_row(bot_tank->get_row_pos());
	field->set_bot_col(bot_tank->get_col_pos());
	field->set_bot_direction(bot_tank->get_direction());

	field->update_field();
}
