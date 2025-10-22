#include "GameEngine.h"
#include "Projectile.h"
#include <vector>
#include <iostream>
#include <memory>

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

void GameEngine::user_shoot() {
	projectiles.push_back(user_tank->shoot());
}

void GameEngine::update_field() {
	move_bot_tank();
	field->set_bot_row(bot_tank->get_row_pos());
	field->set_bot_col(bot_tank->get_col_pos());
	field->set_bot_direction(bot_tank->get_direction());

	for (const auto& p : projectiles) {
		p->move();
	}

	clear_unneeded_projectiles();

	field->update_field(projectiles);
	field->set_bot_hp(bot_tank->get_hp());
	field->set_us_hp(user_tank->get_hp());
}

void GameEngine::clear_unneeded_projectiles() {
	for (size_t i = 0; i < projectiles.size();) {
		int r = projectiles[i]->get_row_pos();
		int c = projectiles[i]->get_col_pos();

		bool hit = false;

		if (field->get_bot_row() == r && field->get_bot_col() == c) {
			bot_tank->lower_hp();
			hit = true;
		}

		else if (field->get_us_row() == r && field->get_us_col() == c) {
			user_tank->lower_hp();
			hit = true;
		}

		if (hit || !field->cell_is_free(r, c)) {
			projectiles.erase(projectiles.begin() + i);
		}
		else i++;
	}
}

bool GameEngine::isGameOver() {
	if (user_tank->get_hp() <= 0) {
		std::cout << "Game over!" << std::endl;
		return true;
	}
	else if (bot_tank->get_hp() <= 0) {
		std::cout << "Victory!" << std::endl;
		return true;
	}
	else return false;
}