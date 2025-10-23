#include "GameEngine.h"
#include "Projectile.h"
#include "Direction.h"
#include <vector>
#include <iostream>
#include <memory>
#include <conio.h>
#include <chrono>

GameEngine::GameEngine() {
	field = std::make_unique<Field>();
	bot_tank = std::make_unique<BotTank>();
	user_tank = std::make_unique<UserTank>();
}

int GameEngine::run() {
	display_field();
	while (true) {

		update_field();
		display_field();
		bot_shoot();

		if (isGameOver()) {
			return 0;
		}

		else if (_kbhit()) {
			int key = _getch();

			if (key == 27) {
				break;
			}

			else if (key == 224) {
				int key_direction = _getch();
				Direction d = Direction::NODIRECTION;
				switch (key_direction) {
				case 72:							//up
					d = Direction::UP;
					break;
				case 80:							//down
					d = Direction::DOWN;
					break;
				case 75:							//left
					d = Direction::LEFT;
					break;
				case 77:							//right
					d = Direction::RIGHT;
					break;
				}
				if (d != Direction::NODIRECTION) {
					move_user_tank(d);
				}
			}

			else if (key == 32) {
				user_shoot();
			}
		}
	}

	return 0;
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

bool GameEngine::bot_shoot() {
	Direction direcction = bot_tank->get_direction();
	int user_r = user_tank->get_row_pos();
	int user_c = user_tank->get_col_pos();

	int bot_r = bot_tank->get_row_pos();
	int bot_c = bot_tank->get_col_pos();

	switch (direcction) {
	case UP:
		if (user_r < bot_r && user_c == bot_c) {
			auto pr = bot_tank->bot_shoot();
			if (pr) projectiles.push_back(std::move(pr));
		}
		return true;
		break;
	case DOWN:
		if (user_r > bot_r && user_c == bot_c) {
			auto pr = bot_tank->bot_shoot();
			if (pr) projectiles.push_back(std::move(pr));
		}
		return true;
		break;
	case LEFT:
		if (user_c < bot_c && user_r == bot_r) {
			auto pr = bot_tank->bot_shoot();
			if (pr) projectiles.push_back(std::move(pr));
		}
		return true;
		break;
	case RIGHT:
		if (user_c > bot_c && user_r == bot_r) {
			auto pr = bot_tank->bot_shoot();
			if (pr) projectiles.push_back(std::move(pr));
		}
		return true;
		break;
	}

	return false;
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