#include "GameEngine.h"
#include "Projectile.h"
#include "Direction.h"
#include "GameFinished.h"
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
#include "GameMode.h"
#include "CellType.h"

class GameEngine : public IGameEngine {
public:
	GameEngine();
	/*void render() override;*/

	GameFinished isGameOver() override;
	const std::vector<std::vector<CellType>>& get_field() const;

	void move_user_tank(Direction direction) override;
	void user_shoot() override;
	bool bot_shoot() override;
	void update_field() override;

	int get_user_hp() override;
	int get_bot_hp(int index) override;

	bool user_is_shot() override;
	bool bot_is_shot(int index) override;

	const std::vector<std::unique_ptr<Projectile>>& get_projectiles() const override;

	Direction get_user_direction() override;
	Direction get_bot_direction(int index) override;

	const std::vector<std::unique_ptr<BotTank>>& get_bot_tanks() const override;
	void start_game(GameMode m) override;
private:
	std::unique_ptr<Field> field;
	std::vector<std::unique_ptr<BotTank>> bot_tanks;
	std::unique_ptr<UserTank> user_tank;

	std::vector<std::unique_ptr<Projectile>> projectiles;
	void clear_unneeded_projectiles();

	void display_field() const;
	void move_bot_tank(int index);

	Keys return_key(int k);

	GameMode mode;
};

void GameEngine::start_game(GameMode m) {
	mode = m;
	if (mode == GameMode::EASY) {
		bot_tanks.push_back(std::make_unique<BotTank>());
	}
	else {
		std::unique_ptr<BotTank> t1 = std::make_unique<BotTank>(2, 2);
		t1->set_direction(Direction::DOWN);
		bot_tanks.push_back(std::move(t1));

		std::unique_ptr<BotTank> t2 = std::make_unique<BotTank>(17, 1);
		t2->set_direction(Direction::LEFT);
		bot_tanks.push_back(std::move(t2));

		std::unique_ptr<BotTank> t3 = std::make_unique<BotTank>(1, 16);
		t3->set_direction(Direction::LEFT);
		bot_tanks.push_back(std::move(t3));

	}
}

GameEngine::GameEngine() {
	field = std::make_unique<Field>();
	user_tank = std::make_unique<UserTank>();
}

std::unique_ptr<IGameEngine> IGameEngine::create_game_engine() {
	return std::make_unique<GameEngine>();
}

//void GameEngine::render() {
//	display_field();
//	while (true) {
//
//		update_field();
//		display_field();
//		bot_shoot();		//int index
//
//		if (isGameOver() == GameFinished::USERLOST) {
//			std::cout << "Game over!" << std::endl;
//			return;
//		}
//		else if (isGameOver() == GameFinished::USERWON) {
//			std::cout << "Victory!" << std::endl;
//			return;
//		}
//
//		else if (_kbhit()) {
//			int key_num = _getch();
//			int key = return_key(key_num);
//
//			if (key == Keys::ESC) {
//				break;
//			}
//
//			else if (key == Keys::ARROW) {
//				int key_dir_num = _getch();
//				int key_direction = return_key(key_dir_num);
//				Direction d = Direction::NODIRECTION;
//				switch (key_direction) {
//				case Keys::AR_UP:
//					d = Direction::UP;
//					break;
//				case Keys::AR_DOWN:
//					d = Direction::DOWN;
//					break;
//				case Keys::AR_LEFT:
//					d = Direction::LEFT;
//					break;
//				case Keys::AR_RIGHT:
//					d = Direction::RIGHT;
//					break;
//				}
//				if (d != Direction::NODIRECTION) {
//					move_user_tank(d);
//				}
//			}
//
//			else if (key == Keys::SPACE) {
//				user_shoot();
//			}
//		}
//	}
//}

void GameEngine::display_field() const {
	system("cls");
	field->display_field(bot_tanks);
}

void GameEngine::move_bot_tank(int index) {
	Direction direction = bot_tanks[index]->get_direction();
	if (direction != Direction::NODIRECTION) {
		int row = bot_tanks[index]->get_row_pos();
		int col = bot_tanks[index]->get_col_pos();
		switch (direction) {
		case UP:
			if (!field->cell_is_free(row - 1, col)) {
				bot_tanks[index]->set_direction(LEFT);
			}
			else bot_tanks[index]->move();
			break;
		case DOWN:
			if (!field->cell_is_free(row + 1, col)) {
				bot_tanks[index]->set_direction(RIGHT);
			}
			else bot_tanks[index]->move();
			break;
		case LEFT:
			if (!field->cell_is_free(row, col - 1)) {
				bot_tanks[index]->set_direction(DOWN);
			}
			else bot_tanks[index]->move();
			break;
		case RIGHT:
			if (!field->cell_is_free(row, col + 1)) {
				bot_tanks[index]->set_direction(UP);
			}
			else bot_tanks[index]->move();
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
	}

}

void GameEngine::user_shoot() {
	projectiles.push_back(user_tank->shoot());
}

bool GameEngine::bot_shoot() {
	bool anyShot = false;
	for (int index = 0;index < bot_tanks.size();index++) {
		Direction direcction = bot_tanks[index]->get_direction();
		int user_r = user_tank->get_row_pos();
		int user_c = user_tank->get_col_pos();

		int bot_r = bot_tanks[index]->get_row_pos();
		int bot_c = bot_tanks[index]->get_col_pos();

		switch (direcction) {
		case UP:
			if (user_r < bot_r && user_c == bot_c) {
				std::unique_ptr<Projectile> pr = bot_tanks[index]->bot_shoot();
				if (pr) projectiles.push_back(std::move(pr));
			}
			anyShot = true;
			break;
		case DOWN:
			if (user_r > bot_r && user_c == bot_c) {
				std::unique_ptr<Projectile> pr = bot_tanks[index]->bot_shoot();
				if (pr) projectiles.push_back(std::move(pr));
			}
			anyShot = true;
			break;
		case LEFT:
			if (user_c < bot_c && user_r == bot_r) {
				std::unique_ptr<Projectile> pr = bot_tanks[index]->bot_shoot();
				if (pr) projectiles.push_back(std::move(pr));
			}
			anyShot = true;
			break;
		case RIGHT:
			if (user_c > bot_c && user_r == bot_r) {
				std::unique_ptr<Projectile> pr = bot_tanks[index]->bot_shoot();
				if (pr) projectiles.push_back(std::move(pr));
			}
			anyShot = true;
			break;
		}
	}

	return anyShot;
}

void GameEngine::update_field() {
	field->update_field(projectiles, bot_tanks);
	for (const auto& p : projectiles) {
		p->move();
	}

	for (int i = 0;i < bot_tanks.size();i++) {
		move_bot_tank(i);
	}

	clear_unneeded_projectiles();

	field->update_field(projectiles, bot_tanks);
	field->set_us_hp(user_tank->get_hp());
}

void GameEngine::clear_unneeded_projectiles() {
	for (size_t i = 0; i < projectiles.size();) {
		int r = projectiles[i]->get_row_pos();
		int c = projectiles[i]->get_col_pos();

		bool hit = false;
		for (int j = 0;j < bot_tanks.size();j++) {
			if (bot_tanks[j]->get_row_pos() == r && bot_tanks[j]->get_col_pos() == c) {
				bot_tanks[j]->lower_hp();
				bot_tanks[j]->set_condition(true);
				bot_tanks[j]->shoot_time = std::chrono::steady_clock::now();
				hit = true;
			}

		}

		if (user_tank->get_row_pos() == r && user_tank->get_col_pos() == c) {
			user_tank->lower_hp();
			user_tank->set_condition(true);
			user_tank->shoot_time = std::chrono::steady_clock::now();
			hit = true;
		}

		if (hit || !field->cell_is_free(r, c)) {
			projectiles.erase(projectiles.begin() + i);
		}
		else i++;
	}
}

GameFinished GameEngine::isGameOver() {
	std::string game_finished_msg = "";
	if (user_tank->get_hp() <= 0) {
		return GameFinished::USERLOST;
	}
	else {
		for (int i = 0;i < bot_tanks.size();) {

			if (bot_tanks[i]->get_hp() <= 0) {
				if (bot_tanks.size() > 1) {
					bot_tanks.erase(bot_tanks.begin() + i);
				}
				else return GameFinished::USERWON;
			}
			else {
				i++;
			}
		}
	}

	return GameFinished::INPROGRESS;
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

const std::vector<std::vector<CellType>>& GameEngine::get_field() const {
	return field->get_field();
}

int GameEngine::get_user_hp() {
	return user_tank->get_hp();
}

int GameEngine::get_bot_hp(int index) {
	return bot_tanks[index]->get_hp();
}

Direction GameEngine::get_user_direction() {
	return user_tank->get_direction();
}

Direction GameEngine::get_bot_direction(int index) {
	return bot_tanks[index]->get_direction();
}

bool GameEngine::user_is_shot() {
	if (user_tank->get_is_shoot()) {
		std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
		long long duration = std::chrono::duration_cast<std::chrono::seconds>(now - user_tank->shoot_time).count();

		if (duration < 1) return true;
		else user_tank->set_condition(false);
	}
	return false;
}

bool GameEngine::bot_is_shot(int index) {
	if (bot_tanks[index]->get_is_shoot()) {
		std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
		long long duration = std::chrono::duration_cast<std::chrono::seconds>(now - bot_tanks[index]->shoot_time).count();

		if (duration < 1) return true;
		else bot_tanks[index]->set_condition(false);
	}
	return false;
}

const std::vector<std::unique_ptr<Projectile>>& GameEngine::get_projectiles() const {
	return projectiles;
}

const std::vector<std::unique_ptr<BotTank>>& GameEngine::get_bot_tanks() const {
	return bot_tanks;
}