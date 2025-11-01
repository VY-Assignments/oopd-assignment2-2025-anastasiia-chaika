#include "GameEngine.h"
#include "Projectile.h"
#include "Direction.h"
#include <vector>
#include <iostream>
#include <memory>
#include <conio.h>
#include <chrono>
#include <string>
#include "Field.h"
#include "BotTank.h"
#include "UserTank.h"
#include "Keys.h"

class GameEngine : public IGameEngine {
public:
	GameEngine();
	void render() override;

	bool isGameOver() override;
	const std::vector<std::vector<char>>& get_field() const;

	void move_user_tank(Direction direction) override;
	void user_shoot() override;
	bool bot_shoot() override;
	void update_field() override;
private:
	std::unique_ptr<Field> field;
	std::unique_ptr<BotTank> bot_tank;
	std::unique_ptr<UserTank> user_tank;

	std::vector<std::unique_ptr<Projectile>> projectiles;
	void clear_unneeded_projectiles();

	void display_field() const;
	void move_bot_tank();

	Keys return_key(int k);
};

GameEngine::GameEngine() {
	field = std::make_unique<Field>();
	bot_tank = std::make_unique<BotTank>();
	user_tank = std::make_unique<UserTank>();

}

std::unique_ptr<IGameEngine> IGameEngine::create_game_engine() {
	return std::make_unique<GameEngine>();
}

void GameEngine::render() {
	display_field();
	while (true) {

		update_field();
		display_field();
		bot_shoot();

		if (isGameOver()) {
			return;
		}

		else if (_kbhit()) {
			int key_num = _getch();
			int key = return_key(key_num);

			if (key == Keys::ESC) {
				break;
			}

			else if (key == Keys::ARROW) {
				int key_dir_num = _getch();
				int key_direction = return_key(key_dir_num);
				Direction d = Direction::NODIRECTION;
				switch (key_direction) {
				case Keys::AR_UP:
					d = Direction::UP;
					break;
				case Keys::AR_DOWN:
					d = Direction::DOWN;
					break;
				case Keys::AR_LEFT:
					d = Direction::LEFT;
					break;
				case Keys::AR_RIGHT:
					d = Direction::RIGHT;
					break;
				}
				if (d != Direction::NODIRECTION) {
					move_user_tank(d);
				}
			}

			else if (key == Keys::SPACE) {
				user_shoot();
			}
		}
	}
}

void GameEngine::display_field() const {
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

Keys GameEngine::return_key(int k) {
	switch (k) {
	case 27:
		return Keys::ESC;
		break;
	case 224: 
		return Keys::ARROW;
	case 72:
		return Keys::AR_UP;
	case 80:
		return Keys::AR_DOWN;
	case 75:
		return Keys::AR_LEFT;
	case 77:
		return Keys::AR_RIGHT;
	case 32:
		return Keys::SPACE;
	}
}

const std::vector<std::vector<char>>& GameEngine::get_field() const {
	return field->get_field();
}